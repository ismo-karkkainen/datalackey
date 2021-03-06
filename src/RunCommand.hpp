//
//  RunCommand.hpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 18.10.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//
// Licensed under Universal Permissive License. See License.txt.

#ifndef RunCommand_hpp
#define RunCommand_hpp

#include "Command.hpp"
#include "Processes.hpp"
#include "Messages.hpp"
#include "CommandDescriptions.hpp"


class RunCommand : public Command {
private:
    Processes& processes;
    RunDescription description;

public:
    RunCommand(const char *const Name, Output& Out, Processes& P);
    ~RunCommand();

    void Perform(const SimpleValue& Id,
        std::vector<std::shared_ptr<SimpleValue>>& Arguments);
};

#endif /* RunCommand_hpp */
