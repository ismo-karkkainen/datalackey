//
//  WaitCommand.hpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 13.12.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#ifndef WaitCommand_hpp
#define WaitCommand_hpp

#include "Command.hpp"
#include "Processes.hpp"


class WaitCommand : public Command {
private:
    Processes& processes;

public:
    WaitCommand(const char *const Name, Output& Out, Processes& P);
    ~WaitCommand();

    void Perform(const SimpleValue& Id, std::vector<SimpleValue*>& Arguments);
};

#endif /* WaitCommand_hpp */
