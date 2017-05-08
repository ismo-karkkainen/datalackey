//
//  StdErr.cpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 12.4.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#include "StdErr.hpp"
#include <iostream>

StdErr::~StdErr() { }

OutputChannel& StdErr::operator<<(const std::vector<char>& Buffer) {
    std::cerr.write(&(Buffer[0]), Buffer.size());
    return *this;
}

void StdErr::Flush() {
    std::cerr.flush();
}
