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

OutputChannel& StdErr::operator<<(const RawData& Buffer) {
    std::cerr.write(Buffer.Raw(), Buffer.Size());
    return *this;
}

void StdErr::Write(RawData::Iterator& Start, RawData::Iterator& End) {
    while (Start != End)
        std::cerr << *Start++;
}

void StdErr::Flush() {
    std::cerr.flush();
}
