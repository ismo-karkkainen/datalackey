//
//  RenameCommand.hpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 5.2.18.
//  Copyright © 2018 Ismo Kärkkäinen. All rights reserved.
//
// Licensed under Universal Permissive License. See License.txt.

#ifndef RenameCommand_hpp
#define RenameCommand_hpp

#include "Command.hpp"
#include "Storage.hpp"
#include "Messages.hpp"
#include "CommandDescriptions.hpp"


class RenameCommand : public Command {
private:
    Storage& storage;
    Sth2Opt2List msg_missing;
    StringListDescription description;

public:
    RenameCommand(const char *const Name, Output& Out, Storage& S);
    ~RenameCommand();

    void Perform(const SimpleValue& Id,
        std::vector<std::shared_ptr<SimpleValue>>& Arguments);
};

#endif /* RenameCommand_hpp */
