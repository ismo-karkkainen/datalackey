//
//  ListCommand.cpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 12.9.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#include "ListCommand.hpp"
#include "Value_t.hpp"
#include "Number_t.hpp"
#include "Messages.hpp"
#include "NullValue.hpp"
#include <memory>


ListCommand::ListCommand(const char *const Name, Output& Out, const Storage& S)
    : Command(Name, Out), storage(S),
    unexpected(Name, "unexpected"), list(Name, "")
{ }

ListCommand::~ListCommand() {
}

void ListCommand::Perform(
    const SimpleValue& Id, std::vector<std::shared_ptr<SimpleValue>>& Arguments)
{
    // An array with output identifier that was given after the command.
    if (!Arguments.empty()) {
        unexpected.Send(out, Id);
        return;
    }
    auto result = storage.List();
    list.Send(out, Id, result);
}
