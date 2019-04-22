//
//  VersionCommand.cpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 11.10.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//
// Licensed under Universal Permissive License. See License.txt.

#include "VersionCommand.hpp"
#include "Version.hpp"
#include "Value_t.hpp"
#include "Number_t.hpp"
#include "NullValue.hpp"
#include "CommandReporter.hpp"
#include <memory>


VersionCommand::VersionCommand(const char *const Name, Output& Out)
    : Command(Name, Out),
    msg_version(Name, "", "mapping"),
    description(Name)
{ }

VersionCommand::~VersionCommand() { }

void VersionCommand::Perform(
    const SimpleValue& Id, std::vector<std::shared_ptr<SimpleValue>>& Arguments)
{
    if (!description.Validate(out, Id, Arguments))
        return;
    std::unique_ptr<OutputItem> writer(out.Writable(IsNullValue(&Id)));
    *writer << Array;
    Message::Feed(*writer, Id);
    *writer << ValueRef<std::string>(Name())
        << ValueRef<std::string>("")
        << Dictionary
        << ValueRef<std::string>("datalackey") << NumberRef<int>(Version)
        << ValueRef<std::string>("interface") << NumberRef<int>(Interface)
        << ValueRef<std::string>("commands")
        << Dictionary;
    CommandReporter::Get().Report(writer.get());
    *writer << End << End << End; // Dictionary, dictionary, and array.
    writer.reset();
    msg_done.Send(out, Id);
}
