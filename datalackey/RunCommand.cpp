//
//  RunCommand.cpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 21.9.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#include "RunCommand.hpp"
#include "Value_t.hpp"
#include "Notifications.hpp"


RunCommand::RunCommand(const char *const Name, Output& Out, Processes& P)
    : Command(Name, Out), processes(P)
{ }

RunCommand::~RunCommand() {
}

bool RunCommand::LabelsOnly() const {
    return false;
}

void RunCommand::Perform(
    const Identifier& Id, std::vector<SimpleValue*>& Arguments)
{
    // An array with run info.
    if (Arguments.empty()) {
        Error(out, Id, "argument", "missing");
        return;
    }
    processes.Run(out, Id, Arguments);
}
