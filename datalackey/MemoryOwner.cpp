//
//  MemoryOwner.cpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 9.1.18.
//  Copyright © 2018 Ismo Kärkkäinen. All rights reserved.
//

#include "MemoryOwner.hpp"
#include "MemoryReader.hpp"
#include <cassert>


MemoryOwner::MemoryOwner()
    : data(new RawData()), finished(false)
{ }

MemoryOwner::~MemoryOwner() { }

bool MemoryOwner::Owner() const {
    return true;
}

void MemoryOwner::Own(bool OwnMemory) { }

bool MemoryOwner::Error() const {
    return false;
}

bool MemoryOwner::Cacheable() const {
    return false;
}

bool MemoryOwner::Finished() const {
    return finished;
}

bool MemoryOwner::Append(RawData::ConstIterator From, RawData::ConstIterator To)
{
    assert(!finished);
    const_cast<RawData*>(data.get())->Append(From, To);
    return true;
}

void MemoryOwner::Finish() {
    assert(!finished);
    finished = true;
}

size_t MemoryOwner::Size() const {
    return data->Size();
}

std::shared_ptr<const RawData> MemoryOwner::FullData() {
    assert(finished);
    return data;
}

std::shared_ptr<DataReader> MemoryOwner::Reader(
    std::shared_ptr<DataOwner>& Owner)
{
    assert(finished);
    return std::shared_ptr<DataReader>(new MemoryReader(Owner));
}
