//
//  GetCommand.hpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 21.9.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//
// Licensed under Universal Permissive License. See License.txt.

#ifndef GetCommand_hpp
#define GetCommand_hpp

#include "Command.hpp"
#include "Storage.hpp"
#include "Messages.hpp"
#include "CommandDescriptions.hpp"


class GetCommand : public Command {
private:
    Storage& storage;
    Sth2Opt2List msg_missing, msg_failed;
    Sth2Opt3 msg_reply; // For message report only.
    StringListDescription description;

public:
    GetCommand(const char *const Name, Output& Out, Storage& S);
    ~GetCommand();

    void Perform(const SimpleValue& Id,
        std::vector<std::shared_ptr<SimpleValue>>& Arguments);
};

#endif /* GetCommand_hpp */
