//
//  RenameCommand.cpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 5.2.18.
//  Copyright © 2018 Ismo Kärkkäinen. All rights reserved.
//

#include "RenameCommand.hpp"
#include "Value_t.hpp"
#include "Notifications.hpp"


RenameCommand::RenameCommand(const char *const Name, Output& Out, Storage& S)
    : Command(Name, Out), storage(S)
{ }

RenameCommand::~RenameCommand() {
}

void RenameCommand::Perform(
    const SimpleValue& Id, std::vector<std::shared_ptr<SimpleValue>>& Arguments)
{
    // An array with output identifier and label pairs.
    if (Arguments.empty()) {
        Error(out, Id, "argument", "missing");
        return;
    }
    if (Arguments.size() % 2 != 0) {
        Error(out, Id, "argument", "pairless");
        return;
    }
    std::vector<std::shared_ptr<SimpleValue>> missing, invalid;
    for (size_t k = 0; k < Arguments.size(); k += 2) {
        StringValue* label(dynamic_cast<StringValue*>(Arguments[k].get()));
        StringValue* target(dynamic_cast<StringValue*>(Arguments[k + 1].get()));
        if (label == nullptr)
            invalid.push_back(Arguments[k]);
        if (target == nullptr)
            invalid.push_back(Arguments[k + 1]);
        if (label == nullptr || target == nullptr)
            continue;
        if (storage.Rename(*label, *target, &out))
            Message(out, Id,
                "renamed", label->String().c_str(), target->String().c_str());
        else
            missing.push_back(Arguments[k]);
    }
    if (!invalid.empty())
        ListMessage(out, Id, "invalid", invalid);
    if (!missing.empty())
        ListMessage(out, Id, "missing", missing);
}
