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


class RunCommand : public Command {
private:
    Processes& processes;

public:
    RunCommand(const char *const Name, Output& Out, Processes& P);
    ~RunCommand();

    bool LabelsOnly() const;
    void Perform(const Identifier& Id, std::vector<SimpleValue*>& Arguments);
};

#endif /* RunCommand_hpp */
