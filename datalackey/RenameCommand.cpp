//
//  RenameCommand.cpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 5.2.18.
//  Copyright © 2018 Ismo Kärkkäinen. All rights reserved.
//

#include "RenameCommand.hpp"
#include "Value_t.hpp"
#include "OutputCollection.hpp"


RenameCommand::RenameCommand(const char *const Name, Output& Out, Storage& S)
    : Command(Name, Out), storage(S),
    msg_missing(Name, "missing"),
    msg_renamed(Name, "renamed"),
    description(Name, true)
{ }

RenameCommand::~RenameCommand() {
}

void RenameCommand::Perform(
    const SimpleValue& Id, std::vector<std::shared_ptr<SimpleValue>>& Arguments)
{
    if (!description.Validate(out, Id, Arguments))
        return;
    std::vector<std::shared_ptr<SimpleValue>> missing, renamed_pairs;
    for (size_t k = 0; k < Arguments.size(); k += 2) {
        StringValue* label(dynamic_cast<StringValue*>(Arguments[k].get()));
        StringValue* target(dynamic_cast<StringValue*>(Arguments[k + 1].get()));
        assert(label != nullptr);
        assert(target != nullptr);
        if (storage.Rename(*label, *target)) {
            renamed_pairs.push_back(Arguments[k]);
            renamed_pairs.push_back(Arguments[k + 1]);
        } else
            missing.push_back(Arguments[k]);
    }
    if (!missing.empty())
        msg_missing.Send(out, Id, missing);
    if (!renamed_pairs.empty()) {
        msg_renamed.Send(out, Id, renamed_pairs);
        DataNotifiedOutputs.Notify(&out,
            [&renamed_pairs](Output* Out) {
                ntf_data_renamed.Send(*Out, renamed_pairs); });
    }
}
