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

streamsize StdIn::Read(std::vector<char>& Buffer) {
    streamsize total = 0;
    while (true) {
        size_t orig = Buffer.size();
        Buffer.resize(orig + ReadBlockSize);
        streamsize count = std::cin.readsome(&(Buffer[orig]), ReadBlockSize);
        total += count;
        if (count < ReadBlockSize) {
            Buffer.resize(orig + count);
            break;
        }
    }
    return total;
}

bool StdIn::Ended() {
    return !std::cin::good();
}
