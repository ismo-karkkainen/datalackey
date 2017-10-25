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

    bool LabelsOnly() const;
    void Perform(const Identifier& Id, std::vector<SimpleValue*>& Arguments);
};

#endif /* DeleteCommand_hpp */
