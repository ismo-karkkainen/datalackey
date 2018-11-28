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
#include "OutputCollection.hpp"


DeleteCommand::DeleteCommand(const char *const Name, Output& Out, Storage& S)
    : Command(Name, Out), storage(S),
    err_missing(Name, "missing"),
    msg_invalid(Name, "invalid"),
    msg_missing(Name, "missing"),
    msg_deleted(Name, "deleted")
{ }

DeleteCommand::~DeleteCommand() {
}

void DeleteCommand::Perform(
    const SimpleValue& Id, std::vector<std::shared_ptr<SimpleValue>>& Arguments)
{
    // An array with output identifier and labels.
    if (Arguments.empty()) {
        err_missing.Send(out, Id);
        return;
    }
    std::vector<std::shared_ptr<SimpleValue>> deleted, missing, invalid;
    for (auto arg : Arguments) {
        StringValue* label(dynamic_cast<StringValue*>(arg.get()));
        if (label != nullptr) {
            if (storage.Delete(*label))
                deleted.push_back(arg);
            else
                missing.push_back(arg);
        } else
            invalid.push_back(arg);
    }
    if (!invalid.empty())
        msg_invalid.Send(out, Id, invalid);
    if (!missing.empty())
        msg_missing.Send(out, Id, missing);
    if (!deleted.empty()) {
        msg_deleted.Send(out, Id, deleted);
        DataNotifiedOutputs.Notify(&out,
            [deleted](Output* Out) { ntf_data_deleted.Send(*Out, deleted); });
    }
}
