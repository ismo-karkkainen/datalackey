//
//  StdOutErr.hpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 29.3.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#ifndef StdOutErr_hpp
#define StdOutErr_hpp

#include "Output.hpp"
#include <vector>
#include <stack>


class StdOutErr : public Output {
    // How about we gather values and when stack becomes empty, output?
    // Or start output as soon as possible and others wait? When there is
    // output for the one, it always prints as soon as possible.

    struct StackBuffer {
        std::stack<MessageType> open;
        std::vector<std::string> buffer;
    };

    std::stack<bool> pair_first;
    StackBuffer structured;
    StackBuffer warnings;
    StackBuffer messages;
    StackBuffer errors; // Own stream but treat the same for simplicity.
    std::stack<StackBuffer*> active;

    std::string indent(size_t levels) const;
public:
    ~StdOutErr();

    StdOutErr& operator<<(const ValueReference& VR);
    StdOutErr& operator<<(MessageType type);
    StdOutErr& Flush();
};

#endif /* StdOutErr_hpp */
