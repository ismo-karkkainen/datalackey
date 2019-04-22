//
//  EndFeedCommand.cpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 31.1.18.
//  Copyright © 2018 Ismo Kärkkäinen. All rights reserved.
//
// Licensed under Universal Permissive License. See License.txt.

#include "EndFeedCommand.hpp"
#include "Value_t.hpp"
#include "Messages.hpp"


EndFeedCommand::EndFeedCommand(
    const char *const Name, Output& Out, Processes& P)
    : Command(Name, Out), processes(P), description(Name)
{ }

EndFeedCommand::~EndFeedCommand() {
}

void EndFeedCommand::Perform(
    const SimpleValue& Id, std::vector<std::shared_ptr<SimpleValue>>& Arguments)
{
    if (!description.Validate(out, Id, Arguments))
        return;
    processes.EndFeed(out, Id, Arguments);
    msg_done.Send(out, Id);
}
