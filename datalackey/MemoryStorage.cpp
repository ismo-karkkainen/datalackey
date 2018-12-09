//
//  MemoryStorage.cpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 10.1.18.
//  Copyright © 2018 Ismo Kärkkäinen. All rights reserved.
//

#include "MemoryStorage.hpp"
#include "MemoryOwner.hpp"
#include "Messages.hpp"
#include "OutputCollection.hpp"
#include <cassert>


MemoryStorage::Value::Value(
    const std::string& Format, std::shared_ptr<DataOwner> Data,
    unsigned long long int Serial)
    : serial(Serial)
{
    values.push_back(std::pair<std::string,std::shared_ptr<DataOwner>>(
        Format, Data));
}

std::shared_ptr<DataReader> MemoryStorage::Value::Find(
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

    // If not throwaway then we have to create a MemoryOwner and push it
    // with format to values. Then pass it to conversion object on construction.
    // We have to create a conversion object and return it.

    // Find suitable source format.
    // if not throwaway, create normal MemoryOwner and push with Format.
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

void MemoryStorage::Value::Own(bool OwnData) {
    for (auto iter : values) {
        iter.second->Own(OwnData);
    }
}


bool MemoryStorage::del(const StringValue& L) {
    auto iter = label2data.find(L);
    if (iter != label2data.end()) {
        std::unique_lock<std::mutex> value_lock(iter->second->Mutex());
        iter->second->Own(true);
        value_lock.unlock();
        label2data.erase(iter);
        return true;
    }
    return false;
}

MemoryStorage::MemoryStorage() : serial(1) { }

MemoryStorage::~MemoryStorage() { }

bool MemoryStorage::IsValid() const {
    return true;
}

std::vector<std::tuple<std::string, unsigned long long int>>
    MemoryStorage::List() const
{
    std::vector<std::tuple<std::string, unsigned long long int>> results;
    std::lock_guard<std::mutex> lock(label2data_mutex);
    results.reserve(label2data.size());
    for (const auto& iter : label2data)
        results.push_back(std::tuple<std::string, unsigned long long int>(
            iter.first.String(), iter.second->Serial()));
    return results;
}

bool MemoryStorage::Delete(const StringValue& L) {
    std::lock_guard<std::mutex> lock(label2data_mutex);
    return del(L);
}

unsigned long long int MemoryStorage::Rename(
    const StringValue& Old, const StringValue& New)
{
    std::lock_guard<std::mutex> lock(label2data_mutex);
    auto old = label2data.find(Old);
    if (old == label2data.end())
        return 0;
    std::shared_ptr<Value> v = old->second;
    label2data.erase(old);
    del(New);
    v->ReSerial(serial++);
    label2data[New] = v;
    return v->Serial();
}

std::vector<std::tuple<std::string, unsigned long long int>> MemoryStorage::Add(
    DataGroup& G)
{
    std::vector<std::tuple<std::string, unsigned long long int>> label_serial;
    label_serial.reserve(G.Size());
    std::lock_guard<std::mutex> lock(label2data_mutex);
    while (true) {
        auto label_data = G.Get();
        if (label_data.second == nullptr)
            break;
        del(label_data.first);
        label_data.second->Own(false);
        std::shared_ptr<Value> v(
            new MemoryStorage::Value(G.Format(), label_data.second, serial++));
        label2data[label_data.first] = v;
        label_serial.push_back(std::make_tuple(
            label_data.first.String(), v->Serial()));
    }
    return label_serial;
}

void MemoryStorage::Prepare(const char *const Format,
    std::vector<std::shared_ptr<ProcessInput>>& Inputs)
{
    std::string fmt(Format);
    std::lock_guard<std::mutex> lock(label2data_mutex);
    for (auto iter : Inputs) {
        if (iter->Label() == nullptr)
            continue; // This one has direct value and is ok already.
        auto source = label2data.find(*(iter->Label()));
        if (source == label2data.end())
            continue; // Missing data will trigger caller to report error.
        std::lock_guard<std::mutex> value_lock(source->second->Mutex());
        iter->SetData(source->second->Find(fmt));
    }
}

std::vector<std::tuple<
    StringValue, std::string, size_t, unsigned long long int>>
    MemoryStorage::Info()
    const
{
    std::vector<std::tuple<
        StringValue, std::string, size_t, unsigned long long int>> results;
    std::lock_guard<std::mutex> lock(label2data_mutex);
    for (auto& iter : label2data) {
        std::lock_guard<std::mutex> value_lock(iter.second->Mutex());
        const auto avail = iter.second->Values();
        for (const auto format_data : avail) {
            results.push_back(std::make_tuple(
                iter.first, format_data.first, format_data.second->Size(),
                iter.second->Serial()));
        }
    }
    return results;
}

DataOwner* MemoryStorage::Generate() {
    return new MemoryOwner();
}
