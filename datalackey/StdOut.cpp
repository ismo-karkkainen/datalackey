//
//  StdOut.cpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 12.4.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#include "StdOut.hpp"
#include <iostream>

StdOut::~StdOut() { }

OutputChannel& StdOut::operator<<(const std::vector<char>& Buffer) {
    std::cout.write(&(Buffer[0]), Buffer.size());
    return *this;
}

void StdOut::Write(Iterator& Start, Iterator& End) {
    while (Start != End)
        std::cout << *Start++;
}

void StdOut::Flush() {
    std::cout.flush();
}
