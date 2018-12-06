//
//  VersionCommand.hpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 11.10.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#ifndef VersionCommand_hpp
#define VersionCommand_hpp

#include "Command.hpp"
#include "Storage.hpp"
#include "Messages.hpp"
#include "CommandDescriptions.hpp"


class VersionCommand : public Command {
private:
    Sth2Opt3 msg_version; // For message report only.
    ParameterlessDescription description;

public:
    VersionCommand(const char *const Name, Output& Out);
    ~VersionCommand();

    void Perform(const SimpleValue& Id,
        std::vector<std::shared_ptr<SimpleValue>>& Arguments);
};

#endif /* VersionCommand_hpp */
