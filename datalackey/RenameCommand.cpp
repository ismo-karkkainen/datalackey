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
#include <map>


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
    std::vector<std::shared_ptr<SimpleValue>> missing;
    std::vector<std::tuple<std::string, unsigned long long int>> renamed;
    // Use map to find out what will end up being deleted and what remains.
    // Using same source label twice shows up in missing values.
    std::map<std::string,unsigned long long int> remains;
    for (size_t k = 0; k < Arguments.size(); k += 2) {
        StringValue* label(dynamic_cast<StringValue*>(Arguments[k].get()));
        StringValue* target(dynamic_cast<StringValue*>(Arguments[k + 1].get()));
        assert(label != nullptr);
        assert(target != nullptr);
        unsigned long long int serial = storage.Rename(*label, *target);
        if (serial) {
            remains[label->String()] = 0ULL;
            remains[target->String()] = serial;
        } else
            missing.push_back(Arguments[k]);
    }
    renamed.reserve(remains.size());
    for (const auto& iter : remains)
        renamed.push_back(std::make_tuple(iter.first, iter.second));
    if (!missing.empty())
        msg_missing.Send(out, Id, missing);
    if (!renamed.empty()) {
        msg_renamed.Send(out, &Id, renamed, true);
        DataNotifiedOutputs.Notify(&out,
            [&renamed](Output* Out) {
                ntf_data_renamed.Send(*Out, renamed, true); });
    }
}
