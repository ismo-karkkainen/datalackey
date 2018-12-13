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
#include "CommandDescriptions.hpp"


class TerminateCommand : public Command {
private:
    Processes& processes;
    Sth2Opt2List msg_missing;
    StringListDescription description;

public:
    TerminateCommand(const char *const Name, Output& Out, Processes& P);
    ~TerminateCommand();

    void Perform(const SimpleValue& Id,
        std::vector<std::shared_ptr<SimpleValue>>& Arguments);
};

#endif /* TerminateCommand_hpp */
