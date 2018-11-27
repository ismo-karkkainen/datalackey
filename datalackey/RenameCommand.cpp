//
//  RenameCommand.cpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 5.2.18.
//  Copyright © 2018 Ismo Kärkkäinen. All rights reserved.
//

#include "RenameCommand.hpp"
#include "Value_t.hpp"


RenameCommand::RenameCommand(const char *const Name, Output& Out, Storage& S)
    : Command(Name, Out), storage(S),
    arg_missing(Name, "missing"),
    pairless(Name, "pairless"),
    msg_renamed(Name, "renamed"),
    msg_invalid(Name, "invalid"),
    msg_missing(Name, "missing")
{ }

RenameCommand::~RenameCommand() {
}

void RenameCommand::Perform(
    const SimpleValue& Id, std::vector<std::shared_ptr<SimpleValue>>& Arguments)
{
    // An array with output identifier and label pairs.
    if (Arguments.empty()) {
        arg_missing.Send(out, Id);
        return;
    }
    if (Arguments.size() % 2 != 0) {
        pairless.Send(out, Id);
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
            msg_renamed.Send(out, Id,
                label->String().c_str(), target->String().c_str());
        else
            missing.push_back(Arguments[k]);
    }
    if (!invalid.empty())
        msg_invalid.Send(out, Id, invalid);
    if (!missing.empty())
        msg_missing.Send(out, Id, missing);
}
