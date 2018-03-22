//
//  NoOperationCommand.cpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 12.9.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#include "NoOperationCommand.hpp"
#include "Messages.hpp"


NoOperationCommand::NoOperationCommand(const char *const Name, Output& Out)
    : Command(Name, Out)
{ }

NoOperationCommand::~NoOperationCommand() {
}

void NoOperationCommand::Perform(
    const SimpleValue& Id, std::vector<std::shared_ptr<SimpleValue>>& Arguments)
{
    Message(out, Id, Name().c_str(), "");
}
