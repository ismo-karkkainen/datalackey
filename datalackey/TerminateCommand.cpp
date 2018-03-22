//
//  TerminateCommand.cpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 21.9.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#include "TerminateCommand.hpp"
#include "Value_t.hpp"
#include "Messages.hpp"


TerminateCommand::TerminateCommand(
    const char *const Name, Output& Out, Processes& P)
    : Command(Name, Out), processes(P)
{ }

TerminateCommand::~TerminateCommand() {
}

void TerminateCommand::Perform(
    const SimpleValue& Id, std::vector<std::shared_ptr<SimpleValue>>& Arguments)
{
    // An array with process identifiers.
    if (Arguments.empty()) {
        Message(out, Id, Name().c_str(), "error", "argument", "missing");
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
        ListMessage(out, Id, Name().c_str(), "missing", missing);
    if (!terminated.empty())
        ListMessage(out, Id, Name().c_str(), "terminated", terminated);
}
