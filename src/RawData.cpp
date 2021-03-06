//
//  RawData.cpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 24.7.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//
// Licensed under Universal Permissive License. See License.txt.

#include "RawData.hpp"


void RawData::Clear(bool LikelyUnused) {
    if (LikelyUnused) {
        std::vector<char> empty;
        buffer.swap(empty);
    } else
        buffer.resize(0);
}

char* RawData::Get(size_t Required) {
    size_t sz = buffer.size();
    buffer.resize(sz + Required);
    return &(buffer[sz]);
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

void RawData::Append(const char *const Item) {
    const char* c = const_cast<const char*>(Item);
    while (*c)
        buffer.push_back(*c++);
}

void RawData::Swap(RawData& RD) {
    buffer.swap(RD.buffer);
}
