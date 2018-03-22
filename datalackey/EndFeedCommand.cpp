//
//  EndFeedCommand.cpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 31.1.18.
//  Copyright © 2018 Ismo Kärkkäinen. All rights reserved.
//

#include "EndFeedCommand.hpp"
#include "Value_t.hpp"
#include "Messages.hpp"


EndFeedCommand::EndFeedCommand(
    const char *const Name, Output& Out, Processes& P)
    : Command(Name, Out), processes(P)
{ }

EndFeedCommand::~EndFeedCommand() {
}

void EndFeedCommand::Perform(
    const SimpleValue& Id, std::vector<std::shared_ptr<SimpleValue>>& Arguments)
{
    if (Arguments.empty()) {
        Message(out, Id, Name().c_str(), "error", "argument", "missing");
        return;
    }
    processes.EndFeed(out, Id, Arguments);
}
