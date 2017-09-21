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


class DeleteCommand : public Command {
private:
    Storage& storage;

public:
    DeleteCommand(const char *const Name, Output& Out, Storage& S);
    ~DeleteCommand();

    void Perform(const std::vector<std::string>& Arguments);
};

#endif /* DeleteCommand_hpp */
