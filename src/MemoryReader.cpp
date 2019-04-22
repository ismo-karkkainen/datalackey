//
//  MemoryReader.cpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 21.1.18.
//  Copyright © 2018 Ismo Kärkkäinen. All rights reserved.
//
// Licensed under Universal Permissive License. See License.txt.

#include "MemoryReader.hpp"
#include <cassert>


MemoryReader::MemoryReader(std::shared_ptr<DataOwner>& Source)
    : source(Source), finished(false)
{ }

MemoryReader::~MemoryReader() { }

bool MemoryReader::Error() const {
    return false;
}

size_t MemoryReader::Size() const {
    return source->Size();
}

std::shared_ptr<const RawData> MemoryReader::Read(size_t SuggestedBlockSize) {
    if (finished)
        return nullptr;
    finished = true;
    return source->FullData();
}
