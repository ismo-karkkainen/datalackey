//
//  FeedCommand.hpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 22.1.18.
//  Copyright © 2018 Ismo Kärkkäinen. All rights reserved.
//

#ifndef FeedCommand_hpp
#define FeedCommand_hpp

#include "Command.hpp"
#include "Processes.hpp"


class FeedCommand : public Command {
private:
    Processes& processes;

public:
    FeedCommand(const char *const Name, Output& Out, Processes& P);
    ~FeedCommand();

    void Perform(const SimpleValue& Id,
        std::vector<std::shared_ptr<SimpleValue>>& Arguments);
};

#endif /* FeedCommand_hpp */
