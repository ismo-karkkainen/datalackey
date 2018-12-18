//
//  DeleteCommand.cpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 21.9.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#include "DeleteCommand.hpp"
#include "Value_t.hpp"
#include "OutputCollection.hpp"


DeleteCommand::DeleteCommand(const char *const Name, Output& Out, Storage& S)
    : Command(Name, Out), storage(S),
    msg_missing(Name, "missing"),
    description(Name)
{ }

DeleteCommand::~DeleteCommand() { }

void DeleteCommand::Perform(
    const SimpleValue& Id, std::vector<std::shared_ptr<SimpleValue>>& Arguments)
{
    if (!description.Validate(out, Id, Arguments))
        return;
    std::vector<std::shared_ptr<SimpleValue>> missing;
    for (auto arg : Arguments) {
        StringValue* label(dynamic_cast<StringValue*>(arg.get()));
        assert(label != nullptr);
        if (!storage.Delete(*label))
            missing.push_back(arg);
    }
    if (!missing.empty())
        msg_missing.Send(out, Id, missing);
    msg_done.Send(out, Id);
}
