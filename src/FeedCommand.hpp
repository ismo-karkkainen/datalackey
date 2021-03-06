//
//  FeedCommand.hpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 22.1.18.
//  Copyright © 2018 Ismo Kärkkäinen. All rights reserved.
//
// Licensed under Universal Permissive License. See License.txt.

#ifndef FeedCommand_hpp
#define FeedCommand_hpp

#include "Command.hpp"
#include "Processes.hpp"
#include "Messages.hpp"
#include "CommandDescriptions.hpp"


class FeedCommand : public Command {
private:
    Processes& processes;
    FeedDescription description;

public:
    FeedCommand(const char *const Name, Output& Out, Processes& P);
    ~FeedCommand();

    void Perform(const SimpleValue& Id,
        std::vector<std::shared_ptr<SimpleValue>>& Arguments);
};

#endif /* FeedCommand_hpp */
