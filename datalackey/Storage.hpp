//
//  Storage.hpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 14.3.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#ifndef Storage_hpp
#define Storage_hpp

#include "StringValue.hpp"
#include "DataGroup.hpp"
#include "ProcessInput.hpp"
#include "Output.hpp"
#include <memory>
#include <vector>
#include <tuple>


// Base class for Storage classes.
class Storage {
public:
    virtual ~Storage();

    virtual bool IsValid() const = 0;

    virtual std::vector<std::string> List() const = 0;
    virtual bool Delete(const StringValue& L, Output* AlreadyNotified = nullptr) = 0;
    virtual bool Rename(const StringValue& Old, const StringValue& New,
        Output* AlreadyNotified = nullptr) = 0;

    virtual void Add(DataGroup& G, Output* AlreadyNotified = nullptr) = 0;

    virtual void Prepare(const char *const Format,
        std::vector<std::shared_ptr<ProcessInput>>& Inputs) = 0;

    // Return label, format, size in bytes.
    virtual std::vector<std::tuple<StringValue,std::string,size_t>> Info() const = 0;
};


#endif /* Storage_hpp */
