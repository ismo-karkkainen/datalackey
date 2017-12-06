//
//  TerminateCommand.cpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 21.9.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#include "TerminateCommand.hpp"
#include "Value_t.hpp"
#include "Notifications.hpp"
#include "NullValue.hpp"


TerminateCommand::TerminateCommand(
    const char *const Name, Output& Out, Processes& P)
    : Command(Name, Out), processes(P)
{ }

TerminateCommand::~TerminateCommand() {
}

void TerminateCommand::Perform(
    const SimpleValue& Id, std::vector<SimpleValue*>& Arguments)
{
    // An array with output identifier and labels.
    if (Arguments.empty()) {
        Error(out, Id, "argument", "missing");
        return;
    }
    bool has_unavailable = false;
    OutputItem* writer = out.Writable(IsNullValue(&Id));
    *writer << Array;
    Feed(*writer, Id);
    for (auto arg : Arguments) {
        if (!processes.Terminate(Id)) {
            if (!has_unavailable) {
                has_unavailable = true;
                *writer << ValueRef<std::string>("error")
                    << ValueRef<std::string>("unavailable");
            }
            Feed(*writer, Id);
        }
    }
    *writer << End;
    delete writer;
}
