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


class VersionCommand : public Command {
public:
    VersionCommand(const char *const Name, Output& Out);
    ~VersionCommand();

    void Perform(const std::vector<std::string>& Arguments);
};

#endif /* VersionCommand_hpp */