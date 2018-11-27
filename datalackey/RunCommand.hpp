//
//  RunCommand.hpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 18.10.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#ifndef RunCommand_hpp
#define RunCommand_hpp

#include "Command.hpp"
#include "Processes.hpp"
#include "Messages.hpp"


class RunCommand : public Command {
private:
    Processes& processes;
    CmdErrorArgumentSth missing;

public:
    RunCommand(const char *const Name, Output& Out, Processes& P);
    ~RunCommand();

    void Perform(const SimpleValue& Id,
        std::vector<std::shared_ptr<SimpleValue>>& Arguments);
};

#endif /* RunCommand_hpp */
