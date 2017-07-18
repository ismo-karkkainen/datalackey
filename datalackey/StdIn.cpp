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

std::streamsize StdIn::Read(std::vector<char>& Buffer) {
    std::streamsize total = 0;
    while (true) {
        size_t orig = Buffer.size();
        Buffer.resize(orig + ReadBlockSize);
        std::cin.read(&(Buffer[orig]), ReadBlockSize);
        std::streamsize count = std::cin.gcount();
        total += count;
        if (count < ReadBlockSize) {
            Buffer.resize(orig + count);
            break;
        }
    }
    return total;
}

bool StdIn::Ended() {
    return !std::cin.good() || std::cin.eof();
}
