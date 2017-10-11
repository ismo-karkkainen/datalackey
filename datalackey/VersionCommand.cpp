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


VersionCommand::VersionCommand(const char *const Name, Output& Out)
    : Command(Name, Out)
{ }

VersionCommand::~VersionCommand() {
}

void VersionCommand::Perform(const std::vector<std::string>& Arguments) {
    // An array with output identifier and labels.
    if (Arguments.size() != 1) {
        Error(out, Arguments[0].c_str(), "argument", "unexpected");
        return;
    }
    OutputItem* writer = out.Writable();
    *writer << Array << ValueRef<std::string>(Arguments[0])
        << Dictionary
        << ValueRef<std::string>("version") << ValueRef<int>(Version)
        << End << End;
    // Maybe add a list of supported formats. And such.
    delete writer;
}
