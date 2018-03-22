//
//  ProcessesCommand.cpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 18.10.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#include "ProcessesCommand.hpp"
#include "Number_t.hpp"
#include "Messages.hpp"
#include "NullValue.hpp"
#include "Value_t.hpp"
#include <memory>


ProcessesCommand::ProcessesCommand(const char *const Name, Output& Out, const Processes& P)
    : Command(Name, Out), processes(P)
{ }

ProcessesCommand::~ProcessesCommand() {
}

void ProcessesCommand::Perform(
    const SimpleValue& Id, std::vector<std::shared_ptr<SimpleValue>>& Arguments)
{
    // An array with output identifier that was given after the command.
    if (!Arguments.empty()) {
        Message(out, Id, Name().c_str(), "error", "argument", "unexpected");
        return;
    }
    auto results = processes.List();
    std::unique_ptr<OutputItem> writer(out.Writable(IsNullValue(&Id)));
    *writer << Array; // Start message array.
    Feed(*writer, Id);
    *writer << ValueRef<std::string>(Name())
        << ValueRef<std::string>("")
        << Dictionary; // Start process id to PID dictionary.
    SimpleValue* id(nullptr);
    pid_t pid;
    for (size_t k = 0; k < results.size(); ++k) {
        std::tie(id, pid) = results[k];
        Feed(*writer, *id);
        *writer << NumberRef<pid_t>(pid);
        delete id;
    }
    *writer << End << End; // Close dictionary and message array.
}
