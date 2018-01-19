//
//  RawDataOwner.cpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 10.1.18.
//  Copyright © 2018 Ismo Kärkkäinen. All rights reserved.
//

#include "RawDataOwner.hpp"
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

bool RawDataOwner::StartRead() {
    assert(finished);
    read = false;
    return true;
}

std::shared_ptr<const RawData> RawDataOwner::Read(size_t SuggestedBlockSize) {
    assert(finished);
    if (read)
        return nullptr;
    read = true;
    return data;
}

void RawDataOwner::FinishRead() {
    assert(finished);
    read = false;
}

RawData* RawDataOwner::Raw() {
    assert(!finished);
    return const_cast<RawData*>(data.get());
}
