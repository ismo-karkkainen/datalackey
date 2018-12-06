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
    : Command(Name, Out), storage(S), list(Name, ""), description(Name)
{ }

ListCommand::~ListCommand() { }

void ListCommand::Perform(
    const SimpleValue& Id, std::vector<std::shared_ptr<SimpleValue>>& Arguments)
{
    if (!description.Validate(out, Id, Arguments))
        return;
    auto result = storage.List();
    list.Send(out, Id, result);
}
