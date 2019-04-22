//
//  TerminateCommand.cpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 21.9.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//
// Licensed under Universal Permissive License. See License.txt.

#include "TerminateCommand.hpp"
#include "Value_t.hpp"


TerminateCommand::TerminateCommand(
    const char *const Name, Output& Out, Processes& P)
    : Command(Name, Out), processes(P),
    msg_missing(Name, "missing"),
    description(Name)
{ }

TerminateCommand::~TerminateCommand() {
}

void TerminateCommand::Perform(
    const SimpleValue& Id, std::vector<std::shared_ptr<SimpleValue>>& Arguments)
{
    if (!description.Validate(out, Id, Arguments))
        return;
    std::vector<std::shared_ptr<SimpleValue>> missing;
    for (auto& arg : Arguments) {
        if (!processes.Terminate(*arg))
            missing.push_back(arg);
    }
    if (!missing.empty())
        msg_missing.Send(out, Id, missing);
    msg_done.Send(out, Id);
}
