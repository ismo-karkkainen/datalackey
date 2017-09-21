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
#include <utility>


class MemoryStorage : public Storage {
private:
    struct FormatData {
        std::string format;
        std::shared_ptr<const RawData> data;
        std::shared_ptr<ConversionResult> receiver;

        FormatData(const char *const Format);
        FormatData(const char *const Format, RawData& Data);

        std::pair<std::shared_ptr<const RawData>,std::shared_ptr<ConversionResult>>
            CheckDataConversion();
    };

    class Values {
    private:
        std::vector<std::shared_ptr<FormatData>> data;
        mutable std::mutex mutex;
    public:
        Values(const char *const Format, RawData& Data);
        ~Values();
        std::mutex& Mutex() const { return mutex; } // Lock this before anything.
        bool IsPresent(const std::string& Format);
        std::vector<std::string> AvailableFormats() const;
        // Returns the item or nullptr.
        std::shared_ptr<const RawData> Get(const std::string& Format);
        std::shared_ptr<const RawData> Get(const std::string& Format) const;
        // Creates the FormatData if not present. Returns nullptr if converted.
        // First indicates that the value was not present but is convertable.
        std::pair<bool,std::shared_ptr<ConversionResult>> Receiver(
            const std::string& Format);
    };
    std::map<std::string,std::shared_ptr<Values>> label2data;
    mutable std::mutex label2data_mutex;

    // Finds the most suitable source format and returns the data or nullptr.
    std::shared_ptr<const RawData> find_source(std::shared_ptr<Values>& Value,
        const std::string& Destination, std::string& SourceFormat);

    // Returns Values and lock to its mutex.
    std::pair<std::shared_ptr<Values>,std::unique_lock<std::mutex>> get(
        const std::string& Label);

    Converter converter;
    size_t used_limit, free_limit;

public:
    MemoryStorage(size_t UsedMegabyteLimit, size_t FreeMegabyteLimit);
    ~MemoryStorage();

    bool IsValid() const;

    std::vector<std::tuple<std::string,std::string,size_t>> List() const;

    void Store(const std::string& Label, const char *const Format,
        RawData& Data);
    bool Delete(const std::string& Label);
    void Clean();

    bool Preload(const std::string& Label, const char *const Format);
    bool IsReady(const std::string& Label, const char *const Format);
    std::shared_ptr<const RawData> Data(const std::string& Label,
        const char *const Format);
    std::shared_ptr<const RawData> ReadyData(const std::string& Label,
        const char *const Format);
};

#endif
