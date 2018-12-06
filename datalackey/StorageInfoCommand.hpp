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
#include "CommandDescriptions.hpp"


class StorageInfoCommand : public Command {
private:
    const Storage& storage;
    Sth2Opt3 msg_info; // For message report only.
    ParameterlessDescription description;

public:
    StorageInfoCommand(const char *const Name, Output& Out, const Storage& S);
    ~StorageInfoCommand();

    void Perform(const SimpleValue& Id,
        std::vector<std::shared_ptr<SimpleValue>>& Arguments);
};

#endif /* StorageInfoCommand_hpp */
