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
#include <mutex>
#include <map>
#include <sys/types.h>
#include <sys/stat.h>


class DirectoryStorage : public Storage, public DataOwnerGenerator {
private:
    class Value {
    private:
        unsigned long long int serial;
        std::vector<std::pair<std::string,std::shared_ptr<DataOwner>>> values;
        std::mutex mutex;

    public:
        Value(unsigned long long int Serial);
        Value(const std::string& Format, std::shared_ptr<DataOwner> Data,
            unsigned long long int Serial);

        // Used when loading data.
        void Add(const std::string& Format, std::shared_ptr<DataOwner> Data);

        std::mutex& Mutex() { return mutex; }
        std::shared_ptr<DataReader> Find(const std::string& Format);
        const std::vector<std::pair<std::string,std::shared_ptr<DataOwner>>>
            Values() const { return values; }
        void Own(bool OwnData);
        unsigned long long int Serial() const { return serial; }
        void ReSerial(unsigned long long int NewSerial) { serial = NewSerial; }
    };

    std::map<StringValue, std::shared_ptr<Value>> label2data;
    unsigned long long int serial;
    mutable std::mutex label2data_mutex;

    // Lock before calling.
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

    std::vector<std::tuple<std::string, unsigned long long int>> List() const;

    bool Delete(const StringValue& L);
    unsigned long long int Rename(
        const StringValue& Old, const StringValue& New);
    std::vector<std::tuple<std::string, unsigned long long int>> Add(
        DataGroup& G);

    void Prepare(const char *const Format,
        std::vector<std::shared_ptr<ProcessInput>>& Inputs);

    std::vector<std::tuple<
        StringValue, std::string, size_t, unsigned long long int>> Info()
            const;

    DataOwner* Generate();
};


#endif /* DirectoryStorage_hpp */
