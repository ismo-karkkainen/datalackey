//
//  RawData.cpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 24.7.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#include "RawData.hpp"


void RawData::Clear(bool LikelyUnused) {
    std::vector<char> empty;
    buffer.swap(empty);
}

char* RawData::Buffer(size_t Required) {
    buffer.resize(buffer.size() + Required);
}

void RawData::Discard(size_t Unused) {
    if (Unused <= buffer.size())
        buffer.resize(buffer.size() - Unused);
    else
        buffer.resize(0);
}

void RawData::Append(ConstIterator From, ConstIterator To) {
    buffer.insert(buffer.end(), From, To);
}

void RawData::Append(const char Item) {
    buffer.push_back(Item);
}
