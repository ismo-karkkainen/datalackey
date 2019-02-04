//
//  RawDataOwner.cpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 10.1.18.
//  Copyright © 2018 Ismo Kärkkäinen. All rights reserved.
//
// Licensed under Universal Permissive License. See License.txt.

#include "RawDataOwner.hpp"
#include "MemoryReader.hpp"
#include <cassert>


RawDataOwner::RawDataOwner()
    : data(new RawData()), finished(false)
{ }

RawDataOwner::~RawDataOwner() { }

bool RawDataOwner::Owner() const {
    return true;
}

void RawDataOwner::Own(bool OwnMemory) { }

bool RawDataOwner::Error() const {
    return false;
}

bool RawDataOwner::Cacheable() const {
    return false;
}

bool RawDataOwner::Finished() const {
    return finished;
}

bool RawDataOwner::Append(
    RawData::ConstIterator From, RawData::ConstIterator To)
{
    assert(!finished);
    return false;
}

void RawDataOwner::Finish() {
    assert(!finished);
    finished = true;
}

size_t RawDataOwner::Size() const {
    return data->Size();
}

std::shared_ptr<const RawData> RawDataOwner::FullData() {
    assert(finished);
    return data;
}

std::shared_ptr<DataReader> RawDataOwner::Reader(
    std::shared_ptr<DataOwner>& Owner)
{
    assert(finished);
    return std::shared_ptr<DataReader>(new MemoryReader(Owner));
}

RawData* RawDataOwner::Raw() {
    assert(!finished);
    return const_cast<RawData*>(data.get());
}
