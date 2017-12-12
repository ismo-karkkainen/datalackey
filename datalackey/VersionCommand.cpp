//
//  VersionCommand.cpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 11.10.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#include "VersionCommand.hpp"
#include "Version.hpp"
#include "Notifications.hpp"
#include "Value_t.hpp"
#include "Number_t.hpp"
#include "NullValue.hpp"
#include <memory>


VersionCommand::VersionCommand(const char *const Name, Output& Out)
    : Command(Name, Out)
{ }

VersionCommand::~VersionCommand() {
}

void VersionCommand::Perform(
    const SimpleValue& Id, std::vector<SimpleValue*>& Arguments)
{
    // An array with output identifier and labels.
    if (!Arguments.empty()) {
        Error(out, Id, "argument", "unexpected");
        return;
    }
    std::unique_ptr<OutputItem> writer(out.Writable(IsNullValue(&Id)));
    *writer << Array;
    Feed(*writer, Id);
    *writer << Dictionary
        << ValueRef<std::string>("version") << NumberRef<int>(Version)
        // Maybe add a list of supported formats. And such.
        << End << End;
}
