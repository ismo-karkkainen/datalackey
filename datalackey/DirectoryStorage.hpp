//
//  DirectoryStorage.hpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 6.2.18.
//  Copyright © 2018 Ismo Kärkkäinen. All rights reserved.
//

#ifndef DirectoryStorage_hpp
#define DirectoryStorage_hpp

#include "Storage.hpp"
#include "DataOwner.hpp"
#include <utility>
#include <string>
#include <memory>
#include <map>
#include <sys/types.h>
#include <sys/stat.h>


class DirectoryStorage : public Storage, public DataOwnerGenerator {
private:
    class Value {
    private:
        std::vector<std::pair<std::string,std::shared_ptr<DataOwner>>> values;
        std::mutex mutex;

    public:
        Value();
        Value(const std::string& Format, std::shared_ptr<DataOwner> Data);

        // Used when loading data.
        void Add(const std::string& Format, std::shared_ptr<DataOwner> Data);

        std::mutex& Mutex() { return mutex; }
        std::shared_ptr<DataReader> Find(const std::string& Format);
        const std::vector<std::pair<std::string,std::shared_ptr<DataOwner>>>
            Values() const { return values; }
        void Own(bool OwnData);
    };

    std::map<StringValue, std::shared_ptr<Value>> label2data;
    mutable std::mutex label2data_mutex;

    bool del(const StringValue& L);

    mode_t file_mode;
    std::string root;

    bool store() const;

public:
    // Should have an output object to report failure to save catalog.
    DirectoryStorage(
        const std::string& StorageRoot, mode_t FileMode = S_IRUSR | S_IWUSR);
    ~DirectoryStorage();

    bool IsValid() const;

    std::vector<std::tuple<StringValue,std::string,size_t>> List() const;

    bool Delete(const StringValue& L, Output* AlreadyNotified = nullptr);
    bool Rename(const StringValue& Old, const StringValue& New,
        Output* AlreadyNotified = nullptr);

    void Add(DataGroup& G, Output* AlreadyNotified = nullptr);

    void Prepare(const char *const Format,
        std::vector<std::shared_ptr<ProcessInput>>& Inputs);

    DataOwner* Generate();
};


#endif /* DirectoryStorage_hpp */
