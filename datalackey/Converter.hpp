//
//  Converter.hpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 31.7.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#ifndef Converter_hpp
#define Converter_hpp

#include "RawData.hpp"
#include <memory>
#include <queue>
#include <mutex>
#include <string>


class ConversionResult {
private:
    RawData* target;
    bool started;
    std::mutex mutex;

public:
    ConversionResult() : target(nullptr), started(false) { }
    ~ConversionResult();
    std::mutex& Mutex() { return mutex; } // Lock using this before anything.
    void Start() { started = true; }
    bool Started() const { return started; }
    void Set(RawData* Target) { target = Target; }
    RawData* Result() { return target; }
};


class Converter {
private:
    struct Task {
        std::shared_ptr<const RawData> data;
        std::string source;
        std::string destination;
        std::shared_ptr<ConversionResult> result;
        Task(std::shared_ptr<const RawData>& Data, const std::string& Source,
            const std::string& Destination,
            std::shared_ptr<ConversionResult> Result);
    };
    // Thread for doing conversion tasks.

    std::queue<Task*> tasks[2];
    std::mutex queue_mutexes[2];

    void enqueue(std::shared_ptr<const RawData>& Data,
        const std::string& Source, const char *const Destination,
        std::shared_ptr<ConversionResult> Result, int Index);
    Task* dequeue();

    // Methods to call to perform the conversion, or external functions?
public:
    Converter();
    ~Converter();

    // Converts immediately.
    RawData* Convert(std::shared_ptr<const RawData> Data,
        const std::string& Source, const char *const Destination);
    // Converts in thread. Urgent decides which queue. Converted is assigned
    // the output pointer once ready.
    void Convert(std::shared_ptr<const RawData> Data, const std::string& Source,
        const char *const Destination, std::shared_ptr<ConversionResult> Result,
        bool Urgent = false)
    {
        enqueue(Data, Source, Destination, Result, Urgent ? 0 : 1);
    }

    // Returns the source formats for Destination, most preferred first.
    std::vector<std::string> PreferredConversionSourceFormats(
        const char *const Destination);
    std::vector<std::string> PreferredConversionSourceFormats(
        const std::string& Destination)
    {
        return PreferredConversionSourceFormats(Destination.c_str());
    }
};

#endif /* Converter.hpp */
