//
//  VersionCommand.cpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 11.10.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#include "VersionCommand.hpp"
#include "Version.hpp"
#include "Messages.hpp"
#include "Value_t.hpp"
#include "Number_t.hpp"
#include "NullValue.hpp"
#include <memory>


VersionCommand::VersionCommand(const char *const Name, Output& Out)
    : Command(Name, Out), unexpected(Name, "unexpected")
{ }

VersionCommand::~VersionCommand() {
}

void VersionCommand::Perform(
    const SimpleValue& Id, std::vector<std::shared_ptr<SimpleValue>>& Arguments)
{
    // An array with output identifier and labels.
    if (!Arguments.empty()) {
        unexpected.Send(out, Id);
        return;
    }
    std::unique_ptr<OutputItem> writer(out.Writable(IsNullValue(&Id)));
    *writer << Array;
    Message::Feed(*writer, Id);
    *writer << ValueRef<std::string>(Name())
        << ValueRef<std::string>("")
        << Dictionary
        << ValueRef<std::string>("datalackey") << NumberRef<int>(Version)
        << ValueRef<std::string>("interface") << NumberRef<int>(Interface)
        << End << End; // Dictionary and array.
}
