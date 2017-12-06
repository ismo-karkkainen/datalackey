//
//  MemoryStorage.cpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 24.7.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#include "MemoryStorage.hpp"
#include <limits>
#include <cassert>
#include <ctime>

/*
static const int keychar_count = 36;
static const char keychars[keychar_count] = "0123456789abcdefghijklmnopqrstuwvxyz";

size_t MemoryStorage::id() {
    while (true) {
        size_t c = counter++;
        std::string cand;
        do {
            cand += keychars[c % keychar_count];
            c /= keychar_count;
        } while (c);
        auto iter = store.find(cand);
        if (iter == store.end())
            return cand;
    }
    // If this becomes an eternal loop, we have more items and memory than
    // size_t can hold. Whee!!!
}
*/

MemoryStorage::FormatData::FormatData(const char *const Format)
    : format(Format), data(nullptr), receiver(new ConversionResult())
{ }

MemoryStorage::FormatData::FormatData(const char *const Format, RawData& Data)
    : format(Format), data(nullptr), receiver(nullptr)
{
    RawData* rd = new RawData();
    rd->Swap(Data);
    data = std::shared_ptr<const RawData>(rd);
}

std::pair<std::shared_ptr<const RawData>,std::shared_ptr<ConversionResult>>
MemoryStorage::FormatData::CheckDataConversion() {
    if (!receiver) {
        return std::make_pair(data, nullptr);
    }
    std::shared_ptr<ConversionResult> tmp = receiver;
    std::lock_guard<std::mutex> lock(tmp->Mutex());
    if (!data) {
        if (receiver->Result()) {
            data = std::shared_ptr<const RawData>(receiver->Result());
            receiver = nullptr;
            return std::make_pair(data, std::shared_ptr<ConversionResult>());
        }
        return std::make_pair(std::shared_ptr<const RawData>(), receiver);
    }
    assert(0); // Should never reach this.
}


MemoryStorage::Values::Values(const char *const Format, RawData& Data) {
    data.push_back(std::shared_ptr<FormatData>(new FormatData(Format, Data)));
}

MemoryStorage::Values::~Values() {
}

bool MemoryStorage::Values::IsPresent(const std::string& Format) {
    for (size_t k = 0; k < data.size(); ++k)
        if (data[k]->format == Format) {
            auto dc = data[k]->CheckDataConversion();
            return dc.second == nullptr;
        }
    return false;
}

std::vector<std::string> MemoryStorage::Values::AvailableFormats() const {
    std::vector<std::string> results;
    for (size_t k = 0; k < data.size(); ++k)
        if (data[k]->data) // Ignore data that is being converted.
            results.push_back(data[k]->format);
    return results;
}

std::shared_ptr<const RawData> MemoryStorage::Values::Get(
    const std::string& Format)
{
    for (size_t k = 0; k < data.size(); ++k)
        if (data[k]->format == Format) {
            auto dc = data[k]->CheckDataConversion();
            return dc.first;
        }
    return nullptr;
}

std::shared_ptr<const RawData> MemoryStorage::Values::Get(
    const std::string& Format) const
{
    for (size_t k = 0; k < data.size(); ++k)
        if (data[k]->format == Format)
            return data[k]->data;
    return nullptr;
}

std::pair<bool,std::shared_ptr<ConversionResult>>
MemoryStorage::Values::Receiver(const std::string& Format) {
    for (size_t k = 0; k < data.size(); ++k)
        if (data[k]->format == Format) {
            auto dc = data[k]->CheckDataConversion();
            return std::make_pair(false, dc.second);
        }
    // There was no matching format.
    data.push_back(std::shared_ptr<FormatData>(new FormatData(Format.c_str())));
    return std::make_pair(true, data.back()->receiver);
}

std::shared_ptr<const RawData> MemoryStorage::find_source(
    std::shared_ptr<MemoryStorage::Values>& Value,
    const std::string& Destination, std::string& SourceFormat)
{
    auto available = Value->AvailableFormats();
    auto order = converter.PreferredConversionSourceFormats(Destination);
    for (size_t idx = 0; idx < order.size(); ++idx)
        for (size_t n = 0; n < available.size(); ++n) {
            if (order[idx] != available[n])
                continue;
            SourceFormat = order[idx];
            return Value->Get(order[idx]);
        }
    return nullptr;
}

std::pair<std::shared_ptr<MemoryStorage::Values>,std::unique_lock<std::mutex>>
MemoryStorage::get(const StringValue& L)
{
    std::unique_lock<std::mutex> lock(label2data_mutex);
    auto iter = label2data.find(L);
    if (iter != label2data.end())
        return std::make_pair(
            iter->second, std::unique_lock<std::mutex>(iter->second->Mutex()));
    lock.unlock();
    return std::make_pair(std::shared_ptr<MemoryStorage::Values>(),
        std::unique_lock<std::mutex>());
}

MemoryStorage::MemoryStorage(size_t UsedMegabyteLimit, size_t FreeMegabyteLimit)
    : used_limit(UsedMegabyteLimit), free_limit(FreeMegabyteLimit)
{ }

MemoryStorage::~MemoryStorage() {
}

bool MemoryStorage::IsValid() const {
    return true;
}

std::vector<std::tuple<StringValue,std::string,size_t>> MemoryStorage::List() const
{
    std::vector<std::tuple<StringValue,std::string,size_t>> results;
    std::lock_guard<std::mutex> lock(label2data_mutex);
    for (auto iter : label2data) {
        std::lock_guard<std::mutex> value_lock(iter.second->Mutex());
        std::vector<std::string> avail = iter.second->AvailableFormats();
        for (auto format : avail) {
            std::shared_ptr<const RawData> data = iter.second->Get(format);
            results.push_back(
                std::make_tuple(iter.first, format, data->Size()));
        }
    }
    return results;
}

void MemoryStorage::Store(const StringValue& L, const char *const Format,
    RawData& Data)
{
    std::shared_ptr<MemoryStorage::Values> val(
        new MemoryStorage::Values(Format, Data));
    std::lock_guard<std::mutex> lock(label2data_mutex);
    label2data[L] = val;
}

bool MemoryStorage::Delete(const StringValue& L) {
    std::lock_guard<std::mutex> lock(label2data_mutex);
    auto iter = label2data.find(L);
    if (iter != label2data.end()) {
        label2data.erase(iter);
        return true;
    }
    return false;
}

void MemoryStorage::Clean() {
    // Nothing to do as user can not mess up with memory.
}

bool MemoryStorage::Preload(const StringValue& L, const char *const Format)
{
    auto vl = get(L);
    if (!vl.first)
        return false; // There is no such label.
    std::shared_ptr<MemoryStorage::Values>& v = vl.first;
    std::string fmt(Format);
    if (v->IsPresent(fmt))
        return true;
    auto cr = v->Receiver(fmt);
    if (!cr.first)
        return true; // Already being converted.
    std::string source_format;
    std::shared_ptr<const RawData> source = find_source(v, fmt, source_format);
    if (source) {
        converter.Convert(source, source_format, Format, cr.second);
        return true;
    }
    // No way to get source, now anyway. Could only happen if chain is needed.
    return false;
}

bool MemoryStorage::IsReady(const StringValue& L, const char *const Format)
{
    std::string fmt(Format);
    auto vl = get(L);
    return vl.first && vl.first->IsPresent(fmt);
}

std::shared_ptr<const RawData> MemoryStorage::Data(const StringValue& L,
    const char *const Format)
{
    std::string fmt(Format);
    auto vl = get(L);
    if (!vl.first)
        return std::shared_ptr<const RawData>(); // No such label.
    std::shared_ptr<MemoryStorage::Values>& v = vl.first;
    std::shared_ptr<const RawData> rd = v->Get(fmt);
    if (rd)
        return rd;
    auto cr = v->Receiver(fmt);
    // If this is first request or conversion has not started, convert now.
    std::unique_lock<std::mutex> lock(cr.second->Mutex());
    if (cr.first || !cr.second->Started()) {
        std::string src_format;
        std::shared_ptr<const RawData> source = find_source(v, fmt, src_format);
        vl.second.unlock();
        if (!source)
            return nullptr; // Cannot get value.
        cr.second->Start();
        lock.unlock();
        RawData* rd = converter.Convert(source, src_format, Format);
        lock.lock();
        cr.second->Set(rd);
        lock.unlock();
        vl.second.lock();
        return v->Get(fmt);
    }
    lock.unlock();
    // Wait for conversion to finish. Then return the result.
    vl.second.unlock();
    while (!rd) {
        struct timespec ts;
        ts.tv_sec = 0;
        ts.tv_nsec = 10000000;
        nanosleep(&ts, nullptr);
        vl.second.lock();
        rd = v->Get(fmt);
        vl.second.unlock();
    }
    return rd;
}

std::shared_ptr<const RawData> MemoryStorage::ReadyData(
    const StringValue& L, const char *const Format)
{
    std::string fmt(Format);
    auto vl = get(L);
    if (!vl.first)
        return std::shared_ptr<const RawData>(); // No such label.
    std::shared_ptr<const RawData> rd = vl.first->Get(fmt);
    if (rd)
        return rd;
    return std::shared_ptr<const RawData>(); // Data not ready.
}
