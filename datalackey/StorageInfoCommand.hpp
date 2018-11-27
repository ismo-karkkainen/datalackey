//
//  StorageInfoCommand.hpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 12.2.18.
//  Copyright © 2018 Ismo Kärkkäinen. All rights reserved.
//

#ifndef StorageInfoCommand_hpp
#define StorageInfoCommand_hpp

#include "Command.hpp"
#include "Storage.hpp"
#include "Messages.hpp"


class StorageInfoCommand : public Command {
private:
    const Storage& storage;
    CmdErrorArgumentSth unexpected;

public:
    StorageInfoCommand(const char *const Name, Output& Out, const Storage& S);
    ~StorageInfoCommand();

    void Perform(const SimpleValue& Id,
        std::vector<std::shared_ptr<SimpleValue>>& Arguments);
};

#endif /* StorageInfoCommand_hpp */
