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
#include <map>


class MemoryStorage : public Storage, public DataOwnerGenerator {
private:
    class Value {
    private:
        std::vector<std::pair<std::string,std::shared_ptr<DataOwner>>> values;
        std::mutex mutex;

    public:
        Value(const std::string& Format, std::shared_ptr<DataOwner> Data);

        std::mutex& Mutex() { return mutex; }
        std::shared_ptr<DataReader> Find(const std::string& Format);
        const std::vector<std::pair<std::string,std::shared_ptr<DataOwner>>>
            Values() const { return values; }
        void Own(bool OwnData);
    };

    std::map<StringValue, std::shared_ptr<Value>> label2data;
    mutable std::mutex label2data_mutex;

    bool del(const StringValue& L);

public:
    MemoryStorage();
    ~MemoryStorage();

    bool IsValid() const;

    std::vector<std::string> List() const;

    bool Delete(const StringValue& L, Output* AlreadyNotified = nullptr);
    bool Rename(const StringValue& Old, const StringValue& New,
        Output* AlreadyNotified = nullptr);

    void Add(DataGroup& G, Output* AlreadyNotified = nullptr);

    void Prepare(const char *const Format,
        std::vector<std::shared_ptr<ProcessInput>>& Inputs);

    std::vector<std::tuple<StringValue,std::string,size_t>> Info() const;

    DataOwner* Generate();
};


#endif /* MemoryStorage_hpp */
