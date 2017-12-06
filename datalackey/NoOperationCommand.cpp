//
//  NoOperationCommand.cpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 12.9.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#include "NoOperationCommand.hpp"
#include "Notifications.hpp"


NoOperationCommand::NoOperationCommand(const char *const Name, Output& Out)
    : Command(Name, Out)
{ }

NoOperationCommand::~NoOperationCommand() {
}

void NoOperationCommand::Perform(
    const SimpleValue& Id, std::vector<SimpleValue*>& Arguments)
{
    for (auto arg : Arguments)
        delete arg;
    Message(out, Id);
}
