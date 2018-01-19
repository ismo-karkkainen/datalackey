//
//  NoOperationCommand.hpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 21.11.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#ifndef NoOperationCommand_hpp
#define NoOperationCommand_hpp

#include "Command.hpp"


class NoOperationCommand : public Command {
public:
    NoOperationCommand(const char *const Name, Output& Out);
    ~NoOperationCommand();

    void Perform(const SimpleValue& Id,
        std::vector<std::shared_ptr<SimpleValue>>& Arguments);
};

#endif /* NoOperationCommand_hpp */
