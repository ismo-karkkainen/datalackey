//
//  FeedCommand.cpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 22.1.18.
//  Copyright © 2018 Ismo Kärkkäinen. All rights reserved.
//

#include "FeedCommand.hpp"
#include "Value_t.hpp"
#include "Notifications.hpp"


FeedCommand::FeedCommand(const char *const Name, Output& Out, Processes& P)
    : Command(Name, Out), processes(P)
{ }

FeedCommand::~FeedCommand() {
}

void FeedCommand::Perform(
    const SimpleValue& Id, std::vector<std::shared_ptr<SimpleValue>>& Arguments)
{
    if (Arguments.empty()) {
        Error(out, Id, "argument", "missing");
        return;
    }
    processes.Feed(out, Id, Arguments);
}
