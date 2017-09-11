//
//  Command.hpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 30.3.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#ifndef Command_hpp
#define Command_hpp

#include "Output.hpp"
#include "json.hpp"
#include <string>


// Base class for command handlers.
class Command {
private:
    Output& out;
    std::string name;

public:
    Command(const char *const Name, Output& Out);
    virtual ~Command();
    const std::string& Name() const { return name; }

    // Perform command, take input in some format. Error message if no command.
    virtual void Perform(nlohmann::json::json& JSONCommand);
    virtual void Help();
};

/*
Incoming array from controller is treated as a command and will be handled by
a derived class. The entire array is passed in and the first word is the
command. An identifier in any case.
*/

#endif /* Command_hpp */
