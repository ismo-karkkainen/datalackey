//
//  RunCommand.cpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 21.9.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#include "RunCommand.hpp"
#include "Value_t.hpp"
#include "Notifications.hpp"


RunCommand::RunCommand(const char *const Name, Output& Out, Processes& P)
    : Command(Name, Out), processes(P)
{ }

RunCommand::~RunCommand() {
}

void RunCommand::Perform(const std::vector<std::string>& Arguments) {
    // An array with output identifier and run info.
    if (Arguments.size() < 2) {
        Error(out, Arguments[0].c_str(), "argument", "missing");
        return;
    }
    std::vector<std::string> result = processes.Run(out, Arguments);
    // Whatever is returned, report it as result.
    OutputItem* writer = out.Writable();
    *writer << Array << ValueRef<std::string>(Arguments[0]);
    for (auto& s : result)
        *writer << ValueRef<std::string>(s);
    *writer << End;
    delete writer;
}
