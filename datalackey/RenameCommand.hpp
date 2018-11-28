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
    Sth2List msg_invalid, msg_missing, msg_renamed;

public:
    RenameCommand(const char *const Name, Output& Out, Storage& S);
    ~RenameCommand();

    void Perform(const SimpleValue& Id,
        std::vector<std::shared_ptr<SimpleValue>>& Arguments);
};

#endif /* RenameCommand_hpp */
