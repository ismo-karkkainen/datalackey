//
//  StorageInfoCommand.cpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 12.2.18.
//  Copyright © 2018 Ismo Kärkkäinen. All rights reserved.
//

#include "StorageInfoCommand.hpp"
#include "Value_t.hpp"
#include "Number_t.hpp"
#include "Messages.hpp"
#include "NullValue.hpp"
#include <memory>


StorageInfoCommand::StorageInfoCommand(
    const char *const Name, Output& Out, const Storage& S)
    : Command(Name, Out), storage(S),
    msg_info(Name, "", "mapping label-to-mapping-format-to-size"),
    description(Name)
{ }

StorageInfoCommand::~StorageInfoCommand() { }

void StorageInfoCommand::Perform(
    const SimpleValue& Id, std::vector<std::shared_ptr<SimpleValue>>& Arguments)
{
    if (!description.Validate(out, Id, Arguments))
        return;
    auto results = storage.Info();
    std::unique_ptr<OutputItem> writer(out.Writable(IsNullValue(&Id)));
    *writer << Array; // Start message array.
    Message::Feed(*writer, Id);
    *writer << ValueRef<std::string>(Name())
        << ValueRef<std::string>("")
        << Dictionary; // Start label dictionary.
    StringValue label("");
    std::string format;
    size_t size;
    long long int serial;
    if (!results.empty()) {
        std::tie(label, format, size, serial) = results[0];
        *writer << ValueRef<std::string>(label) << Dictionary
            << ValueRef<std::string>("serial")
            << NumberRef<long long int>(serial)
            << ValueRef<std::string>(format) << NumberRef<size_t>(size);
        long long int previous(serial);
        for (size_t k = 1; k < results.size(); ++k) {
            std::tie(label, format, size, serial) = results[k];
            if (previous != serial) {
                // Close previous format:size dictionary and start new.
                *writer << End
                    << ValueRef<std::string>(label) << Dictionary
                    << ValueRef<std::string>("serial")
                    << NumberRef<long long int>(serial);
                previous = serial;
            }
            *writer << ValueRef<std::string>(format) << NumberRef<size_t>(size);
        }
        *writer << End; // Close previous format:size dictionary.
    }
    *writer << End << End; // Close label dictionary and message array.
}
