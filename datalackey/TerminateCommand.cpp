//
//  TerminateCommand.cpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 21.9.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#include "TerminateCommand.hpp"
#include "Value_t.hpp"


TerminateCommand::TerminateCommand(
    const char *const Name, Output& Out, Processes& P)
    : Command(Name, Out), processes(P),
    arg_missing(Name, "missing"),
    msg_missing(Name, "missing"),
    msg_terminated(Name, "terminated")
{ }

TerminateCommand::~TerminateCommand() {
}

void TerminateCommand::Perform(
    const SimpleValue& Id, std::vector<std::shared_ptr<SimpleValue>>& Arguments)
{
    // An array with process identifiers.
    if (Arguments.empty()) {
        arg_missing.Send(out, Id);
        return;
    }
    std::vector<std::shared_ptr<SimpleValue>> terminated, missing;
    for (auto arg : Arguments) {
        if (processes.Terminate(*arg))
            terminated.push_back(arg);
        else
            missing.push_back(arg);
    }
    if (!missing.empty())
        msg_missing.Send(out, Id, missing);
    if (!terminated.empty())
        msg_terminated.Send(out, Id, terminated);
}
