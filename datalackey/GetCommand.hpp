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


class GetCommand : public Command {
private:
    Storage& storage;
    std::string format;

public:
    GetCommand(const char *const Name, Output& Out, Storage& S,
        const char *const Format);
    ~GetCommand();

    void Perform(const std::vector<std::string>& Arguments);
};

#endif /* GetCommand_hpp */
