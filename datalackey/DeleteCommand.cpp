//
//  DeleteCommand.cpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 21.9.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#include "DeleteCommand.hpp"
#include "Value_t.hpp"
#include "Notifications.hpp"


DeleteCommand::DeleteCommand(const char *const Name, Output& Out, Storage& S)
    : Command(Name, Out), storage(S)
{ }

DeleteCommand::~DeleteCommand() {
}

bool DeleteCommand::LabelsOnly() const {
    return true;
}

void DeleteCommand::Perform(
    const Identifier& Id, std::vector<SimpleValue*>& Arguments)
{
    // An array with output identifier and labels.
    if (Arguments.empty()) {
        Error(out, Id, "argument", "missing");
        return;
    }
    // Check if everything can be made available and if not, return an error.
    std::vector<Label*> unavailable;
    for (auto arg : Arguments) {
        Label* label(dynamic_cast<Label*>(arg));
        assert(label != nullptr);
        if (storage.Delete(*label))
            delete arg;
        else
            unavailable.push_back(label);
    }
    if (!unavailable.empty()) {
        OutputItem* writer = out.Writable();
        *writer << Array
            << ValueRef<std::string>("error");
        Feed(*writer, Id);
        *writer << ValueRef<std::string>("unavailable");
        for (auto s : unavailable) {
            Feed(*writer, s);
            delete s;
        }
        *writer << End;
        delete writer;
        return;
    }
    OutputItem* writer = out.Writable();
    *writer << Array;
    Feed(*writer, Id);
    *writer << End;
    delete writer;
}
