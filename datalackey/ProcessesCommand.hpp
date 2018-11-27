//
//  ProcessesCommand.hpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 18.10.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#ifndef ProcessesCommand_hpp
#define ProcessesCommand_hpp

#include "Command.hpp"
#include "Processes.hpp"
#include "Messages.hpp"


class ProcessesCommand : public Command {
private:
    const Processes& processes;
    CmdErrorArgumentSth unexpected;

public:
    ProcessesCommand(const char *const Name, Output& Out, const Processes& P);
    ~ProcessesCommand();

    void Perform(const SimpleValue& Id,
        std::vector<std::shared_ptr<SimpleValue>>& Arguments);
};

#endif /* ProcessesCommand_hpp */
