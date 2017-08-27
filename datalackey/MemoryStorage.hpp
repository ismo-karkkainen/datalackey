//
//  MemoryStorage.hpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 24.7.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#ifndef MemoryStorage_hpp
#define MemoryStorage_hpp

#include "Storage.hpp"
#include "Converter.hpp"
#include <map>
#include <vector>
#include <string>
#include <memory>
#include <mutex>
#include <utlity>


class MemoryStorage : public Storage {
private:
    struct FormatData {
        std::string format;
        std::shared_ptr<const RawData> data;
        std::shared_ptr<ConversionResult> receiver;

        FormatData(const std::string& Format,
            std::shared_ptr<const RawData>& Data)
            : format(Format), data(Data) { }
        FormatData(const std::string& Format)
            : format(Format) { }

        std::pair<std::shared_ptr<const RawData>,std::shared_ptr<ConversionResult>>
            CheckDataConversion();
    };

    class Values {
    private:
        std::vector<shared_ptr<FormatData>> data;
        std::mutex mutex;
    public:
        Values();
        ~Values();
        std::mutex& Mutex() { return mutex; } // Lock this before anything.
        bool IsPresent(const std::string& Format);
        std::vector<std::string> AvailableFormats();
        // Returns the item or nullptr.
        shared_ptr<const RawData> Get(const std::string& Format);
        // Creates the FormatData if not present. Returns nullptr if converted.
        // First indicates that the value was not present but is convertable.
        std::pair<bool,std::shared_ptr<ConversionReceiver>> Receiver(
            const std::string& Format);
    };
    std::map<std::string,shared_ptr<Values>> label2data;
    std::mutex label2data_mutex;

    // Finds the most suitable source format and returns the data or nullptr.
    shared_ptr<const RawData> find_source(shared_ptr<Values>& Value,
        const std::string& Destination, std::string& SourceFormat);

    // Returns Values and lock to its mutex.
    std::pair<shared_ptr<Values>,std::unique_lock<std::mutex>> get(
        const std::string& Label);

    Converter converter;

public:
    MemoryStorage();
    ~MemoryStorage();

    bool IsValid() const;

    void Store(const std::string& Label, RawData& Data);
    void Delete(const std::string& Label);
    void Clean();

    void Preload(const std::string& Label, const char *const Format);
    bool IsReady(const std::string& Label, const char *const Format);
    shared_ptr<const RawData> Data(const std::string& Label,
        const char *const Format);
};
