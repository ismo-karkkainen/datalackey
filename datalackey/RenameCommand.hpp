//
//  RenameCommand.hpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 5.2.18.
//  Copyright © 2018 Ismo Kärkkäinen. All rights reserved.
//

#ifndef RenameCommand_hpp
#define RenameCommand_hpp

#include "Command.hpp"
#include "Storage.hpp"
#include "Messages.hpp"


class RenameCommand : public Command {
private:
    Storage& storage;
    CmdErrorArgumentSth arg_missing;
    CmdErrorArgumentSth pairless;
    CmdSthArg2 msg_renamed;
    CmdSthList msg_invalid, msg_missing;

public:
    RenameCommand(const char *const Name, Output& Out, Storage& S);
    ~RenameCommand();

    void Perform(const SimpleValue& Id,
        std::vector<std::shared_ptr<SimpleValue>>& Arguments);
};

#endif /* RenameCommand_hpp */
