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

OutputChannel& StdOut::operator<<(const RawData& Buffer) {
    std::cout.write(Buffer.Raw(), Buffer.Size());
    return *this;
}

void StdOut::Write(RawData::ConstIterator& Start, RawData::ConstIterator& End) {
    while (Start != End)
        std::cout << *Start++;
}

void StdOut::Flush() {
    std::cout.flush();
}
