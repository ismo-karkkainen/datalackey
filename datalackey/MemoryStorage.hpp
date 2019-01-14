//
//  MemoryStorage.hpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 10.1.18.
//  Copyright © 2018 Ismo Kärkkäinen. All rights reserved.
//

#ifndef MemoryStorage_hpp
#define MemoryStorage_hpp

#include "Storage.hpp"
#include "DataOwner.hpp"
#include <utility>
#include <string>
#include <memory>
#include <mutex>
#include <map>


class MemoryStorage : public Storage, public DataOwnerGenerator {
private:
    class Value {
    private:
        long long int serial;
        std::vector<std::pair<std::string,std::shared_ptr<DataOwner>>> values;
        std::mutex mutex;

    public:
        Value(const std::string& Format, std::shared_ptr<DataOwner> Data,
            long long int Serial);

        std::mutex& Mutex() { return mutex; }
        std::shared_ptr<DataReader> Find(const std::string& Format);
        const std::vector<std::pair<std::string,std::shared_ptr<DataOwner>>>
            Values() const { return values; }
        void Own(bool OwnData);
        long long int Serial() const { return serial; }
        void ReSerial(long long int NewSerial) { serial = NewSerial; }
    };

    std::map<StringValue, std::shared_ptr<Value>> label2data;
    long long int serial;
    mutable std::mutex label2data_mutex;

    long long int del(const StringValue& L);

public:
    MemoryStorage();
    ~MemoryStorage();

    bool IsValid() const;

    std::vector<std::pair<std::string, long long int>> List() const;

    bool Delete(const StringValue& L);
    bool Rename(const StringValue& Old, const StringValue& New);
    void Add(DataGroup& G);

    void Prepare(const char *const Format,
        std::vector<std::shared_ptr<ProcessInput>>& Inputs);

    std::vector<std::tuple<StringValue, std::string, size_t, long long int>>
        Info() const;

    DataOwner* Generate();
};


#endif /* MemoryStorage_hpp */
