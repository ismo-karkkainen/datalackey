//
//  StdIn.cpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 24.5.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#include "StdIn.hpp"
#include <iostream>


const size_t ReadBlockSize = 65536;

StdIn::~StdIn() {
}

std::streamsize StdIn::Read(RawData& Buffer) {
    std::streamsize total = 0;
    while (true) {
        char* buf = Buffer.Get(ReadBlockSize);
        std::cin.read(buf, ReadBlockSize);
        std::streamsize count = std::cin.gcount();
        total += count;
        if (count < ReadBlockSize) {
            Buffer.Discard(ReadBlockSize - count);
            break;
        }
    }
    return total;
}

bool StdIn::Ended() {
    return !std::cin.good() || std::cin.eof();
}
