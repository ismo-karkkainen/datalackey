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

bool ProcessesCommand::LabelsOnly() const {
    return false;
}

void ProcessesCommand::Perform(
    const Identifier& Id, std::vector<SimpleValue*>& Arguments)
{
    // An array with output identifier that was given after the command.
    if (!Arguments.empty()) {
        Error(out, Id, "argument", "unexpected");
        for (auto arg : Arguments)
            delete arg;
        return;
    }
    auto results = processes.List();
    OutputItem* writer = out.Writable();
    *writer << Array; // Start message array.
    Feed(*writer, Id);
    *writer << Dictionary; // Start process id to PID dictionary.
    Identifier id(0);
    pid_t pid;
    for (size_t k = 0; k < results.size(); ++k) {
        std::tie(id, pid) = results[k];
        Feed(*writer, id);
        *writer << ValueRef<pid_t>(pid);
    }
    *writer << End << End; // Close dictionary and message array.
    delete writer;
}
