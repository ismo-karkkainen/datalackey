//
//  DirectoryStorage.cpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 6.2.18.
//  Copyright © 2018 Ismo Kärkkäinen. All rights reserved.
//
// Licensed under Universal Permissive License. See License.txt.

#include "DirectoryStorage.hpp"
#include "FileOwner.hpp"
#include "Messages.hpp"
#include "OutputCollection.hpp"
#include "FileDescriptor.hpp"
#include <nlohmann/json.hpp>
#include <dirent.h>
#include <fcntl.h>
#include <unistd.h>
#include <cerrno>
#include <fstream>
#include <algorithm>
#include <utility>
#include <cassert>


using json = nlohmann::json;

static const char Catalog[] = "catalog";


DirectoryStorage::Value::Value(long long int Serial)
    : serial(Serial)
{ }

DirectoryStorage::Value::Value(
    const std::string& Format, std::shared_ptr<DataOwner> Data,
    long long int Serial)
    : serial(Serial)
{
    Add(Format, Data);
}

void DirectoryStorage::Value::Add(
    const std::string& Format, std::shared_ptr<DataOwner> Data)
{
    values.push_back(std::pair<std::string,std::shared_ptr<DataOwner>>(
        Format, Data));
}

std::shared_ptr<DataReader> DirectoryStorage::Value::Find(
    const std::string& Format)
{
    bool throwaway = false;
    for (auto iter : values)
        if (iter.first == Format) {
            if (iter.second->Finished())
                return iter.second->Reader(iter.second);
            throwaway = true;
            break;
        }

    // Before CBOR is implemented, we can never get here.
    assert(false);
    return std::shared_ptr<DataReader>();

    // If not throwaway then we have to create a DirectoryOwner and push it
    // with format to values. Then pass it to conversion object on construction.
    // We have to create a conversion object and return it.

    // Find suitable source format.
    // if not throwaway, create normal DirectoryOwner and push with Format.
    // Take source format, its DataOwner, non-throwaway, targte format and
    // generate converter and return it.

    // However, a conversion has to be "finished" since it must be readable
    // from the requester. So the above case applies when we have conversion
    // result storing object feeding to what we found. We cannot have an object
    // that feeds to itself since the requester has to have something finished.
    // So a single converter is always finished but never here. Here is the
    // conversion storage as by-product. For purposes of freeing stuff the
    // object here should not know about the converter so that it will be
    // freed. Or converter tells object here to free reference to converter
    // when reading is finished (i.e. we read the last block).

    // Asking for same is presumably rare and pointer storage is small and
    // looking for the conversion is fast, now at least.
    // Note that there is no guarantee that the conversion will ever be
    // finished unless we do it explicitly disregarding what requester wants
    // and perform conversion until the end in any case. For memory storage
    // we could allow arbitrary number of readers as long as they wait for more
    // data and that implies locking the source to the degree that writes and
    // reads do not overlap (but reads can overlap).
    // For file storage read before write is finished ought to be a no-no.
    // For file storage a thread would have to start converting immediately.
    // Maybe same for memory storage for simplicity.
    // For file storage we also have only one reader in turn. Separate reader
    // object that holds a smart pointer to owner?
    // Actually they can all be derived from DataOwner.

    // Separate writer and reader objects to handle data owner contents?
    // Might help for FileOwner but anything purely in memory returns pointer
    // to everything so there is nothing for reader.

    // If the multiple reads for file become a problem, orchestrate reads
    // through DataOwners that share file content data. Or just non-owning
    // read-only object.
}

void DirectoryStorage::Value::Own(bool OwnData) {
    for (auto& iter : values) {
        iter.second->Own(OwnData);
    }
}


long long int DirectoryStorage::del(const StringValue& L) {
    auto iter = label2data.find(L);
    if (iter != label2data.end()) {
        std::unique_lock<std::mutex> value_lock(iter->second->Mutex());
        iter->second->Own(true);
        long long int serial = iter->second->Serial();
        value_lock.unlock();
        label2data.erase(iter);
        return serial;
    }
    return 0;
}

bool DirectoryStorage::store() const {
    json j(json::object());
    std::unique_lock<std::mutex> lock(label2data_mutex);
    // Find order from smallest serial to largest.
    std::vector<std::pair<long long int, const StringValue*>> order;
    order.reserve(label2data.size());
    for (const auto& iter : label2data)
         order.push_back(std::make_pair(iter.second->Serial(), &(iter.first)));
    std::sort(order.begin(), order.end());
    for (const auto& item : order) {
        auto iter = label2data.find(*item.second);
        const std::shared_ptr<Value> v(iter->second);
        json arr(json::array());
        std::unique_lock<std::mutex> value_lock(v->Mutex());
        for (const auto& pair : v->Values()) {
            arr.push_back(pair.first);
            FileOwner* fo = dynamic_cast<FileOwner*>(pair.second.get());
            assert(fo != nullptr);
            arr.push_back(fo->Index());
        }
        value_lock.unlock();
        j[item.second->String()] = arr;
    }
    lock.unlock();
    std::ofstream out(root + Catalog);
    out << j;
    return out.good();
}

DirectoryStorage::DirectoryStorage(
    const std::string& StorageRoot, mode_t FileMode)
    : serial(1), file_mode(FileMode), root(StorageRoot)
{
    // Trust main program to hand us a valid data directory.
    // See if there is a catalog to be loaded. May be new storage.
    std::string catalog = root;
    catalog += Catalog;
    std::unique_ptr<FileDescriptor> fd(FileDescriptor::Open(catalog.c_str()));
    if (fd == nullptr) {
        switch (errno) {
        case EACCES:
        case ELOOP:
        case EMFILE:
        case ENAMETOOLONG:
        case ENOTDIR:
        case EOVERFLOW:
            root.clear();
            break;
        }
        // There is no catalog file to read. New storage.
        if (!store())
            root.clear(); // Try writing to find out we can.
        return;
    }
    // fd is valid so read the catalog into label2data.
    struct stat info;
    if (-1 == fstat(fd->Descriptor(), &info)) {
        root.clear();
        return;
    }
    std::unique_ptr<char[]> buffer(new char[info.st_size + 1]);
    errno = 0;
    ssize_t r = read(fd->Descriptor(), buffer.get(), info.st_size);
    fd->Close();
    if (r != info.st_size) {
        root.clear();
        return;
    }
    buffer[r] = 0;
    json j;
    try {
        j = json::parse(buffer.get());
    }
    catch (const std::exception& e) {
        root.clear();
        // This means we have invalid catalog and should report somehow.
        return;
    }
    buffer = nullptr;
    for (json::iterator item = j.begin(); item != j.end(); ++item) {
        StringValue label(item.key());
        std::shared_ptr<Value> value(new Value(serial++));
        label2data[label] = value;
        for (size_t k = 0; k < item.value().size(); k += 2) {
            std::string format = item.value()[k];
            size_t index = item.value()[k + 1];
            value->Add(format,
                std::shared_ptr<DataOwner>(new FileOwner(root, index)));
        }
    }
}

DirectoryStorage::~DirectoryStorage() {
    if (IsValid())
        store();
}

bool DirectoryStorage::IsValid() const {
    return !root.empty();
}

std::vector<std::pair<std::string, long long int>> DirectoryStorage::List()
    const
{
    std::vector<std::pair<std::string, long long int>> results;
    std::lock_guard<std::mutex> lock(label2data_mutex);
    results.reserve(label2data.size());
    for (const auto& iter : label2data)
        results.push_back(std::pair<std::string, long long int>(
            iter.first.String(), iter.second->Serial()));
    return results;
}

bool DirectoryStorage::Delete(const StringValue& L) {
    std::unique_lock<std::mutex> lock(label2data_mutex);
    long long int item_serial = del(L);
    lock.unlock();
    if (item_serial) {
        NotifyDelete(L.String(), item_serial);
        return true;
    }
    return false;
}

bool DirectoryStorage::Rename(
    const StringValue& Old, const StringValue& New)
{
    std::unique_lock<std::mutex> lock(label2data_mutex);
    auto old = label2data.find(Old);
    if (old == label2data.end())
        return false;
    std::shared_ptr<Value> v = old->second;
    label2data.erase(old);
    del(New);
    long long int old_serial = v->Serial();
    v->ReSerial(serial++);
    label2data[New] = v;
    lock.unlock();
    NotifyDelete(Old.String(), old_serial);
    NotifyStore(New.String(), v->Serial());
    return true;
}

void DirectoryStorage::Add(DataGroup& G) {
    std::vector<std::pair<std::string, long long int>> messages;
    messages.reserve(G.Size());
    std::unique_lock<std::mutex> lock(label2data_mutex);
    while (true) {
        auto label_data = G.Get();
        if (label_data.second == nullptr)
            break;
        del(label_data.first);
        label_data.second->Own(false);
        std::shared_ptr<Value> v(new DirectoryStorage::Value(
            G.Format(), label_data.second, serial++));
        label2data[label_data.first] = v;
        messages.push_back(std::pair<std::string, long long int>(
            label_data.first.String(), v->Serial()));
    }
    lock.unlock();
    for (auto& iter : messages)
        NotifyStore(iter.first, iter.second);
}

void DirectoryStorage::Prepare(const char *const Format,
    std::vector<std::shared_ptr<ProcessInput>>& Inputs)
{
    std::string fmt(Format);
    std::lock_guard<std::mutex> lock(label2data_mutex);
    for (auto& iter : Inputs) {
        if (iter->Label() == nullptr)
            continue; // This one has direct value and is ok already.
        auto source = label2data.find(*(iter->Label()));
        if (source == label2data.end())
            continue; // Missing data will trigger caller to report error.
        std::lock_guard<std::mutex> value_lock(source->second->Mutex());
        iter->SetData(source->second->Find(fmt));
    }
}

std::vector<std::tuple<StringValue, std::string, size_t, long long int>>
    DirectoryStorage::Info() const
{
    std::vector<std::tuple<
        StringValue, std::string, size_t, long long int>> results;
    std::lock_guard<std::mutex> lock(label2data_mutex);
    for (auto& iter : label2data) {
        std::lock_guard<std::mutex> value_lock(iter.second->Mutex());
        const auto avail = iter.second->Values();
        for (const auto& format_data : avail) {
            results.push_back(std::make_tuple(
                iter.first, format_data.first, format_data.second->Size(),
                iter.second->Serial()));
        }
    }
    return results;
}

DataOwner* DirectoryStorage::Generate() {
    return new FileOwner(root, file_mode);
}
