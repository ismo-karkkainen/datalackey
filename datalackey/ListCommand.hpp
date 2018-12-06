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
#include "Messages.hpp"
#include "CommandDescriptions.hpp"


class ListCommand : public Command {
private:
    const Storage& storage;
    Sth2Opt2List list;
    ParameterlessDescription description;

public:
    ListCommand(const char *const Name, Output& Out, const Storage& S);
    ~ListCommand();

    void Perform(const SimpleValue& Id,
        std::vector<std::shared_ptr<SimpleValue>>& Arguments);
};

#endif /* ListCommand_hpp */
