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


TerminateCommand::TerminateCommand(
    const char *const Name, Output& Out, Processes& P)
    : Command(Name, Out), processes(P)
{ }

TerminateCommand::~TerminateCommand() {
}

void TerminateCommand::Perform(const std::vector<std::string>& Arguments) {
    // An array with output identifier and labels.
    if (Arguments.size() < 2) {
        Error(out, Arguments[0].c_str(), "argument", "missing");
        return;
    }
    // Check if everything can be made available and if not, return an error.
    std::vector<std::string> unavailable;
    for (size_t k = 1; k < Arguments.size(); ++k) {
        if (!processes.Terminate(Arguments[k]))
            unavailable.push_back(Arguments[k]);
    }
    if (!unavailable.empty()) {
        OutputItem* writer = out.Writable();
        *writer << Array
            << ValueRef<std::string>("error")
            << ValueRef<std::string>(Arguments[0])
            << ValueRef<std::string>("unavailable");
        for (auto s : unavailable)
            *writer << ValueRef<std::string>(s);
        *writer << End;
        delete writer;
        return;
    }
    OutputItem* writer = out.Writable();
    *writer << Array
        << ValueRef<std::string>(Arguments[0])
        << End;
    delete writer;
}
