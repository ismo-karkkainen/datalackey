//
//  Storage.hpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 14.3.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//
// Licensed under Universal Permissive License. See License.txt.

#ifndef Storage_hpp
#define Storage_hpp

#include "StringValue.hpp"
#include "DataGroup.hpp"
#include "ProcessInput.hpp"
#include "Output.hpp"
#include <memory>
#include <vector>
#include <utility>


// Base class for Storage classes.
class Storage {
public:
    virtual ~Storage();

    virtual bool IsValid() const = 0;

    virtual std::vector<std::pair<std::string, long long int>> List()
        const = 0;

    virtual bool Delete(const StringValue& L) = 0;
    virtual bool Rename(const StringValue& Old, const StringValue& New) = 0;
    virtual void Add(DataGroup& G) = 0;

    virtual void Prepare(const char *const Format,
        std::vector<std::shared_ptr<ProcessInput>>& Inputs) = 0;

    // Return label, format, size in bytes, serial.
    virtual std::vector<std::tuple<
        StringValue, std::string, size_t, long long int>> Info() const = 0;

    // Methods that do not depend on what the storage is.

    // Send notification to all or the given output.
    void NotifyDelete(const std::string& Label, long long int Serial,
        Output* Out = nullptr) const;
    void NotifyStore(const std::string& Label, long long int Serial,
        Output* Out = nullptr) const;
    void NotifyError(const std::string& Label, long long int Serial,
        Output* Out = nullptr) const;
};


#endif /* Storage_hpp */
