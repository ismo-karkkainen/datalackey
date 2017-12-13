//
//  WaitCommand.cpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 13.12.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#include "WaitCommand.hpp"
#include "Value_t.hpp"
#include "Notifications.hpp"


WaitCommand::WaitCommand(
    const char *const Name, Output& Out, Processes& P)
    : Command(Name, Out), processes(P)
{ }

WaitCommand::~WaitCommand() {
}

void WaitCommand::Perform(
    const SimpleValue& Id, std::vector<SimpleValue*>& Arguments)
{
    // An array with output identifiers.
    if (Arguments.empty()) {
        Error(out, Id, "argument", "missing");
        return;
    }
    std::vector<SimpleValue*> terminated, missing;
    for (auto arg : Arguments) {
        if (processes.Wait(*arg))
            terminated.push_back(arg);
        else
            missing.push_back(arg);
    }
    if (!missing.empty())
        ListMessage(out, Id, "missing", missing);
    if (!terminated.empty())
        ListMessage(out, Id, "terminated", terminated);
}
