//
//  FeedCommand.cpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 22.1.18.
//  Copyright © 2018 Ismo Kärkkäinen. All rights reserved.
//
// Licensed under Universal Permissive License. See License.txt.

#include "FeedCommand.hpp"
#include "Value_t.hpp"
#include "Messages.hpp"


FeedCommand::FeedCommand(const char *const Name, Output& Out, Processes& P)
    : Command(Name, Out), processes(P), description(Name)
{ }

FeedCommand::~FeedCommand() { }

void FeedCommand::Perform(
    const SimpleValue& Id, std::vector<std::shared_ptr<SimpleValue>>& Arguments)
{
    if (!description.Validate(out, Id, Arguments))
        return;
    processes.Feed(out, Id, Arguments);
    msg_done.Send(out, Id);
}
