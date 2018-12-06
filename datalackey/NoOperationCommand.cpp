//
//  NoOperationCommand.cpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 12.9.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#include "NoOperationCommand.hpp"


NoOperationCommand::NoOpMessage::NoOpMessage(const char* const Name)
    : name(Name)
{ }

void NoOperationCommand::NoOpMessage::Report(Output& Out) const {
    Send(Out, Message::id);
}

void NoOperationCommand::NoOpMessage::Send(Output& Out, const SimpleValue& Id)
    const
{
    message(Out, Id, name, "");
}


NoOperationCommand::NoOperationCommand(const char *const Name, Output& Out)
    : Command(Name, Out), reply(Name), description(Name)
{ }

NoOperationCommand::~NoOperationCommand() { }

void NoOperationCommand::Perform(
    const SimpleValue& Id, std::vector<std::shared_ptr<SimpleValue>>& Arguments)
{
    if (!description.Validate(out, Id, Arguments))
        return;
    reply.Send(out, Id);
}
