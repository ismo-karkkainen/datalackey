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
#include <vector>
#include <string>


// Base class for command handlers.
class Command {
protected:
    Output& out;

private:
    std::string name;

public:
    Command(const char *const Name, Output& Out);
    virtual ~Command();
    const std::string& Name() const { return name; }

    virtual void Perform(const std::vector<std::string>& Arguments) = 0;
};


#endif /* Command_hpp */
