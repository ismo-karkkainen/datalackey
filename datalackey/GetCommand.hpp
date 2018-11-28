//
//  GetCommand.hpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 21.9.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#ifndef GetCommand_hpp
#define GetCommand_hpp

#include "Command.hpp"
#include "Storage.hpp"
#include "Messages.hpp"


class GetCommand : public Command {
private:
    Storage& storage;
    CmdErrorArgumentSth arg_missing;
    Sth2List msg_invalid, msg_missing, msg_failed;

public:
    GetCommand(const char *const Name, Output& Out, Storage& S);
    ~GetCommand();

    void Perform(const SimpleValue& Id,
        std::vector<std::shared_ptr<SimpleValue>>& Arguments);
};

#endif /* GetCommand_hpp */
