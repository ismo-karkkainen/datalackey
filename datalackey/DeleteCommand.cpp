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
#include <cassert>


DeleteCommand::DeleteCommand(const char *const Name, Output& Out, Storage& S)
    : Command(Name, Out), storage(S)
{ }

DeleteCommand::~DeleteCommand() {
}

void DeleteCommand::Perform(
    const SimpleValue& Id, std::vector<SimpleValue*>& Arguments)
{
    // An array with output identifier and labels.
    if (Arguments.empty()) {
        Error(out, Id, "argument", "missing");
        return;
    }
    // Output should be always something for each value, even deletions.
    for (auto arg : Arguments) {
        StringValue* label(dynamic_cast<StringValue*>(arg));
        if (label == nullptr) {
            Error(out, Id, "argument", "not-string");
            return;
        }
    }
    bool has_unavailable = false;
    OutputItem* writer = out.Writable();
    *writer << Array;
    Feed(*writer, Id);
    for (auto arg : Arguments) {
        StringValue* label(dynamic_cast<StringValue*>(arg));
        if (label == nullptr) {
            Error(out, Id, "argument", "not-string");
            return;
        }
        if (!storage.Delete(*label)) {
            if (!has_unavailable) {
                has_unavailable = true;
                *writer << ValueRef<std::string>("error")
                    << ValueRef<std::string>("unavailable");
            }
            Feed(*writer, *label);
        }
        delete arg;
    }
    *writer << End;
    delete writer;
}
