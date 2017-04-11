//
//  StdOutErr.cpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 29.3.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#include "StdOutErr.hpp"
#include <iostream>

std::string StdOutErr::indent(size_t levels) const {
    return std::string(2 * levels, ' ');
}

StdOutErr::~StdOutErr() { }

StdOutErr& StdOutErr::operator<<(const ValueReference& VR) {
    if (active.top() == &structured) {
        if (structured.open.top() != Dictionary)
            structured.buffer.push_back(indent(structured.open.size()));
        else {
            if (pair_first.top())
                structured.buffer.push_back(indent(structured.open.size()));
            else
                structured.buffer.push_back(" = ");
        }
    }
    active.top()->buffer.push_back(VR.String());
    if (active.top() == &structured) {
        if (structured.open.top() == Dictionary) {
            bool first = pair_first.top();
            if (!first)
                structured.buffer.push_back("\n");
            pair_first.pop();
            pair_first.push(!first);
        } else
            structured.buffer.push_back("\n");
    }
    return *this;
}

StdOutErr& StdOutErr::operator<<(MessageType type) {
    // Errors can go out as they are.
    // Messages and structures should wait for each other.
    // If inside something, the message waits. Needs to be stored temporarily.
    switch (type) {
    case Message:
        active.push(&messages);
        messages.open.push(Message);
        break;
    case Warning:
        active.push(&warnings);
        warnings.open.push(Warning);
        break;
    case Error:
        active.push(&errors);
        errors.open.push(Error);
        break;
    case Array:
        active.push(&structured);
        structured.open.push(Array);
        structured.buffer.push_back(indent(structured.open.size()) + "List:\n");
        break;
    case Dictionary:
        active.push(&structured);
        structured.open.push(Dictionary);
        pair_first.push(true);
        break;
    case End:
        if (active.empty()) {
            std::cerr << "<nothing started but End encountered>\n";
            break;
        }
        if (active.top()->open.top() == Dictionary) {
            if (pair_first.top())
                structured.buffer.push_back("<missing dictionary value>\n");
            pair_first.pop();
        } else if (active.top() != &structured)
            active.top()->buffer.push_back("\n"); // Terminate message.
            // This creates funny output on nested messages.
        active.top()->open.pop();
        if (active.top()->open.empty()) {
            // Fully closed a general type so output it all.
            for (const std::string& s : active.top()->buffer)
                if (active.top() != &errors)
                    std::cout << s;
                else
                    std::cerr << s;
            active.top()->buffer.resize(0);
        }
        active.pop();
        break;
    }
    return *this;
}

StdOutErr& StdOutErr::Flush() {
    std::cout.flush();
    std::cerr.flush();
    return *this;
}
