//
//  DeleteCommand.cpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 21.9.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#include "DeleteCommand.hpp"
#include "Value_t.hpp"
#include "Messages.hpp"


DeleteCommand::DeleteCommand(const char *const Name, Output& Out, Storage& S)
    : Command(Name, Out), storage(S)
{ }

DeleteCommand::~DeleteCommand() {
}

void DeleteCommand::Perform(
    const SimpleValue& Id, std::vector<std::shared_ptr<SimpleValue>>& Arguments)
{
    // An array with output identifier and labels.
    if (Arguments.empty()) {
        Message(out, Id, Name().c_str(), "error", "argument", "missing");
        return;
    }
    std::vector<std::shared_ptr<SimpleValue>> deleted, missing, invalid;
    for (auto arg : Arguments) {
        StringValue* label(dynamic_cast<StringValue*>(arg.get()));
        if (label != nullptr) {
            if (storage.Delete(*label, &out))
                deleted.push_back(arg);
            else
                missing.push_back(arg);
        } else
            invalid.push_back(arg);
    }
    if (!invalid.empty())
        ListMessage(out, Id, Name().c_str(), "invalid", invalid);
    if (!missing.empty())
        ListMessage(out, Id, Name().c_str(), "missing", missing);
    if (!deleted.empty())
        ListMessage(out, Id, Name().c_str(), "deleted", deleted);
}
