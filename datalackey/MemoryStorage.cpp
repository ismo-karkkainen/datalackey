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


std::pair<std::shared_ptr<const RawData>,std::shared_ptr<ConversionResult>>
MemoryStorage::FormatData::CheckDataConversion() {
    if (!receiver) {
        return std::make_pair<std::shared_ptr<const RawData>,std::shared_ptr<ConversionResult>>(data, nullptr);
    }
    std::shared_ptr<ConversionResult> tmp = receiver;
    std::lock_guard<std::mutex> lock(tmp->Mutex());
    if (!data) {
        if (receiver->Result()) {
            data = receiver->Result();
            receiver = nullptr;
            return std::make_pair<std::shared_ptr<const RawData>,std::shared_ptr<ConversionResult>>(data, nullptr);
        }
        return std::make_pair<std::shared_ptr<const RawData>,std::shared_ptr<ConversionResult>>(nullptr, receiver);
    }
    assert(0); // Should never reach this.
}


MemoryStorage::Values::Values() {
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

std::vector<std::string> MemoryStorage::Values::AvailableFormats() {
    std::vector<std::string> results;
    for (size_t k = 0; k < data.size(); ++k)
        if (data[k].data) // Ignore data that is being converted.
            results.push_back(data[k]->format);
    return results;
}

shared_ptr<const RawData> MemoryStorage::Values::Get(const std::string& Format)
{
    for (size_t k = 0; k < data.size(); ++k)
        if (data[k]->format == Format) {
            auto dc = data[k]->CheckDataConversion();
            return dc.first;
        }
    return nullptr;
}

std::pair<bool,std::shared_ptr<ConversionReceiver>>
MemoryStorage::Values::Receiver(const std::string& Format) {
    for (size_t k = 0; k < data.size(); ++k)
        if (data[k]->format == Format) {
            auto dc = data[k]->CheckDataConversion();
            return std::make_pair<bool,std::shared_ptr<ConversionReceiver>>(
                false, dc.second);
        }
    // There was no matching format.
    data.push_back(new FormatData(Format));
    return std::make_pair<bool,std::shared_ptr<ConversionReceiver>>(
        true, data.back()->receiver);
}

shared_ptr<const RawData> MemoryStorage::find_source(shared_ptr<Values>& Value,
    const std::string& Destination, std::string& SourceFormat)
{
    auto available = v.AvailableFormats();
    auto order = converter.PreferredConversionSourceFormats(Destination);
    for (size_t idx = 0; idx < order.size(); ++idx)
        for (size_t n = 0; n < available.size(); ++n) {
            if (order[idx] != available[n])
                continue;
            SourceFormat = order[idx];
            return v.Get(order[idx]);
        }
    return nullptr;
}

std::pair<shared_ptr<Values>,std::unique_lock<std::mutex>> MemoryStorage::get(
    const std::string& Label)
{
    std::unique_lock<std::mutex> lock(label2data_mutex);
    auto iter = label2data.find(Label);
    if (iter != label2data.end())
        return std::make_pair<shared_ptr<Values>,std::unique_lock<std::mutex>>(
            iter->second, std::unique_lock<std::mutex>(iter->second->Mutex()));
    lock.unlock();
    return std::make_pair<shared_ptr<Values>,std::unique_lock<std::mutex>>(
        shared_ptr<Values>(),std::unique_lock<std::mutex>());
}

MemoryStorage::MemoryStorage() {
}

MemoryStorage::~MemoryStorage() {
}

bool MemoryStorage::IsValid() const {
    return true;
}

void MemoryStorage::Store(const std::string& Label, RawData& Data) {
    shared_ptr<Values> val = new Values();
    val->Append(Format, Data);
    std::lock_guard<std::mutex> lock(label2data_mutex);
    label2data[Label] = val;
}

void MemoryStorage::Delete(const std::string& Label) {
    std::lock_guard<std::mutex> lock(label2data_mutex);
    auto iter = label2data.find(Label);
    if (iter != label2data.end()) {
        label2data.erase(iter);
    }
}

void MemoryStorage::Clean() {
    // Nothing to do as user can not mess up with memory.
}

void MemoryStorage::Preload(const std::string& Label, const char *const Format)
{
    auto vl = get(Label);
    shared_ptr<Values>& v = vl.first;
    std::string fmt(Format);
    if (v.IsPresent(fmt))
        return;
    auto cr = v.Receiver(fmt);
    if (!cr.first)
        return; // Already being converted.
    std::string source_format;
    shared_ptr<const RawData> source = find_source(v, fmt, source_format);
    if (source)
        converter.Convert(source, source_format, Format, cr.second);
    // There was no way to perform the conversion. At the time anyway.
}

bool MemoryStorage::IsReady(const std::string& Label, const char *const Format)
{
    std::string fmt(Format);
    auto vl = get(Label);
    return vl.first->IsPresent(fmt);
}

shared_ptr<const RawData> MemoryStorage::Data(const std::string& Label,
    const char *const Format)
{
    std::string fmt(Format);
    auto vl = get(Label);
    shared_ptr<Values>& v = vl.first;
    shared_ptr<const RawData> rd = v.Get(fmt);
    if (rd)
        return rd;
    auto cr = v.Receiver(fmt);
    // If this is first request or conversion has not started, convert now.
    std::unique_lock<std::mutex> lock(cr.second->Mutex());
    if (cr.first || !cr.second->Started()) {
        std::string src_format;
        shared_ptr<const RawData> source = find_source(v, fmt, src_format);
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
        return v.Get(fmt);
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
        rd = v.Get(fmt);
        vl.second.unlock();
    }
    return rd;
}
