//
//  ProcessesCommand.cpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 18.10.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#include "ProcessesCommand.hpp"
#include "Value_t.hpp"
#include "Notifications.hpp"


ProcessesCommand::ProcessesCommand(const char *const Name, Output& Out, const Processes& P)
    : Command(Name, Out), processes(P)
{ }

ProcessesCommand::~ProcessesCommand() {
}

void ProcessesCommand::Perform(const std::vector<std::string>& Arguments) {
    // An array with output identifier that was given after the command.
    if (Arguments.size() != 1) {
        Error(out, Arguments[0].c_str(), "argument", "unexpected");
        return;
    }
    auto results = processes.List();
    OutputItem* writer = out.Writable();
    *writer << Array; // Start message array.
    *writer << ValueRef<std::string>(Arguments[0]);
    *writer << Dictionary; // Start process id to PID dictionary.
    std::string id;
    pid_t pid;
    for (size_t k = 0; k < results.size(); ++k) {
        std::tie(id, pid) = results[k];
        *writer << ValueRef<std::string>(id) << ValueRef<pid_t>(pid);
    }
    *writer << End << End; // Close dictionary and message array.
    delete writer;
}
