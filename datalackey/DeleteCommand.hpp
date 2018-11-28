//
//  DeleteCommand.hpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 21.9.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#ifndef DeleteCommand_hpp
#define DeleteCommand_hpp

#include "Command.hpp"
#include "Storage.hpp"
#include "Messages.hpp"


class DeleteCommand : public Command {
private:
    Storage& storage;
    CmdErrorArgumentSth err_missing;
    Sth2List msg_invalid, msg_missing, msg_deleted;

public:
    DeleteCommand(const char *const Name, Output& Out, Storage& S);
    ~DeleteCommand();

    void Perform(const SimpleValue& Id,
        std::vector<std::shared_ptr<SimpleValue>>& Arguments);
};

#endif /* DeleteCommand_hpp */
