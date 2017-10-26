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
    // Check if everything can be made available and if not, return an error.
    std::vector<Identifier*> unavailable;
    for (auto arg : Arguments) {
        Identifier* id(dynamic_cast<Identifier*>(arg));
        assert(id != nullptr);
        if (processes.Terminate(*id))
            delete id;
        else
            unavailable.push_back(id);
    }
    if (!unavailable.empty()) {
        OutputItem* writer = out.Writable();
        *writer << Array
            << ValueRef<std::string>("error");
        Feed(*writer, Id);
        *writer << ValueRef<std::string>("unavailable");
        for (auto arg : unavailable) {
            Feed(*writer, *arg);
            delete arg;
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
