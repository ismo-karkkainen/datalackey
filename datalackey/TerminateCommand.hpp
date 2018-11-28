//
//  TerminateCommand.hpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 18.10.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#ifndef TerminateCommand_hpp
#define TerminateCommand_hpp

#include "Command.hpp"
#include "Processes.hpp"
#include "Messages.hpp"


class TerminateCommand : public Command {
private:
    Processes& processes;
    CmdErrorArgumentSth arg_missing;
    Sth2List msg_missing, msg_terminated;

public:
    TerminateCommand(const char *const Name, Output& Out, Processes& P);
    ~TerminateCommand();

    void Perform(const SimpleValue& Id,
        std::vector<std::shared_ptr<SimpleValue>>& Arguments);
};

#endif /* TerminateCommand_hpp */
