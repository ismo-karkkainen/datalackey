//
//  Converter.cpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 1.8.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#include "Converter.hpp"


ConversionResult::~ConversionResult() {
    delete target;
}

Converter::Task::Task(std::shared_ptr<const RawData>& Data,
    const std::string& Source, const std::string& Destination,
    std::shared_ptr<ConversionResult> Result)
    : data(Data), source(Source), destination(Destination), result(Result)
{ }

void Converter::enqueue(std::shared_ptr<const RawData>& Data,
    const std::string& Source, const char *const Destination,
    std::shared_ptr<ConversionResult> Result, int Index)
{
    std::lock_guard<std::mutex> lock(queue_mutexes[Index]);
    tasks[Index].push(new Task(Data, Source, std::string(Destination), Result));
}

Converter::Task* Converter::dequeue() {
    for (int k = 0; k < 2; ++k) {
        std::lock_guard<std::mutex> lock(queue_mutexes[k]);
        if (!tasks[k].empty()) {
            Task* t = tasks[k].front();
            tasks[k].pop();
            return t;
        }
    }
    return nullptr;
}

Converter::Converter() {
    // Start thread now or when first task comes?
}

Converter::~Converter() {
    // Stop thread.
    for (int k = 0; k < 2; ++k)
        while (!tasks[k].empty()) {
            delete tasks[k].front();
            tasks[k].pop();
        }
}

RawData* Converter::Convert(std::shared_ptr<const RawData> Data,
    const std::string& Source, const char *const Destination)
{
    // Call conversion method directly.
    return nullptr;
}

std::vector<std::string> Converter::PreferredConversionSourceFormats(
    const char *const Destination)
{
    std::vector<std::string> results;
    /*
    if (!strcmp("JSON", Destination)) {
        results.push_back("CBOR");
    } else if (!strcmp("CBOR", Destination)) {
        results.push_back("JSON");
    }
    */
    return results;
}
