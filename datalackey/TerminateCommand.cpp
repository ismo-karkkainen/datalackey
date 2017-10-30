//
//  TerminateCommand.cpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 21.9.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#include "TerminateCommand.hpp"
#include "Value_t.hpp"
#include "Notifications.hpp"
#include <cassert>


TerminateCommand::TerminateCommand(
    const char *const Name, Output& Out, Processes& P)
    : Command(Name, Out), processes(P)
{ }

TerminateCommand::~TerminateCommand() {
}

bool TerminateCommand::LabelsOnly() const {
    return false;
}

void TerminateCommand::Perform(
    const Identifier& Id, std::vector<SimpleValue*>& Arguments)
{
    // An array with output identifier and labels.
    if (Arguments.empty()) {
        Error(out, Id, "argument", "missing");
        return;
    }
    bool has_unavailable = false;
    OutputItem* writer = out.Writable();
    *writer << Array;
    Feed(*writer, Id);
    for (auto arg : Arguments) {
        Identifier* id(dynamic_cast<Identifier*>(arg));
        assert(id != nullptr);
        if (!processes.Terminate(*id)) {
            if (!has_unavailable) {
                has_unavailable = true;
                *writer << ValueRef<std::string>("error")
                    << ValueRef<std::string>("unavailable");
            }
            Feed(*writer, *id);
        }
        delete id;
    }
    *writer << End;
    delete writer;
}
