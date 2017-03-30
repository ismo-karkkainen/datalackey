//
//  MessageOutput.cpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 16.3.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#include "StdOutErr.hpp"
#include <iostream>

StdOutErr::~StdOutErr() { }

StdOutErr& StdOutErr::operator<<(const std::string& s) {
    if (out)
        std::cout << s;
    else
        std::cerr << s;
    return *this;
}

StdOutErr& StdOutErr::operator<<(const char* c) {
    if (out)
        std::cout << c;
    else
        std::cerr << c;
    return *this;
}

StdOutErr& StdOutErr::operator<<(MessageType type) {
    out = type != Error;
    return *this;
}

StdOutErr& StdOutErr::Flush() {
    std::cout.flush();
    std::cerr.flush();
    return *this;
}
