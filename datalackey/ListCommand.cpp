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
#include "Notifications.hpp"
#include "NullValue.hpp"
#include <memory>


ListCommand::ListCommand(const char *const Name, Output& Out, const Storage& S)
    : Command(Name, Out), storage(S)
{ }

ListCommand::~ListCommand() {
}

void ListCommand::Perform(
    const SimpleValue& Id, std::vector<std::shared_ptr<SimpleValue>>& Arguments)
{
    // An array with output identifier that was given after the command.
    if (!Arguments.empty()) {
        Error(out, Id, "argument", "unexpected");
        return;
    }
    auto result = storage.List();
    ListMessage(out, Id, nullptr, result);
}
