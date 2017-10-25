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


class TerminateCommand : public Command {
private:
    Processes& processes;

public:
    TerminateCommand(const char *const Name, Output& Out, Processes& P);
    ~TerminateCommand();

    bool LabelsOnly() const;
    void Perform(const Identifier& Id, std::vector<SimpleValue*>& Arguments);
};

#endif /* TerminateCommand_hpp */
