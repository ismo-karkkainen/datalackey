//
//  EndFeedCommand.hpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 31.1.18.
//  Copyright © 2018 Ismo Kärkkäinen. All rights reserved.
//

#ifndef EndFeedCommand_hpp
#define EndFeedCommand_hpp

#include "Command.hpp"
#include "Processes.hpp"
#include "Messages.hpp"
#include "CommandDescriptions.hpp"


class EndFeedCommand : public Command {
private:
    Processes& processes;
    StringListDescription description;

public:
    EndFeedCommand(const char *const Name, Output& Out, Processes& P);
    ~EndFeedCommand();

    void Perform(const SimpleValue& Id,
        std::vector<std::shared_ptr<SimpleValue>>& Arguments);
};

#endif /* EndFeedCommand_hpp */
