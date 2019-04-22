//
//  Command.hpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 30.3.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//
// Licensed under Universal Permissive License. See License.txt.

#ifndef Command_hpp
#define Command_hpp

#include "Output.hpp"
#include "SimpleValue.hpp"
#include <vector>
#include <string>
#include <memory>


// Base class for commands.
class Command {
protected:
    Output& out;

private:
    std::string name;

public:
    Command(const char *const Name, Output& Out);
    virtual ~Command();
    const std::string& Name() const { return name; }

    virtual void Perform(const SimpleValue& Id,
        std::vector<std::shared_ptr<SimpleValue>>& Arguments) = 0;
};


#endif /* Command_hpp */
