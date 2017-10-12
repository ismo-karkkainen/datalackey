//
//  ListCommand.hpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 12.9.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#ifndef ListCommand_hpp
#define ListCommand_hpp

#include "Command.hpp"
#include "Storage.hpp"


class ListCommand : public Command {
private:
    const Storage& storage;

public:
    ListCommand(const char *const Name, Output& Out, const Storage& S);
    ~ListCommand();

    void Perform(const std::vector<std::string>& Arguments);
};

#endif /* ListCommand_hpp */