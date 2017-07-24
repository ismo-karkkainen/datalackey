//
//  StorageDataSinkJSON.cpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 26.5.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#include "StorageDataSinkJSON.hpp"


StorageDataSinkJSON::StorageDataSinkJSON(Storage& S)
    : storage(S)
{ }

StorageDataSinkJSON::~StorageDataSinkJSON() {
}

const char *const StorageDataSinkJSON::Format() const {
    return "JSON";
}

void StorageDataSinkJSON::Input(RawData::Iterator& Start, RawData::Iterator& End)
{
    // Split to key, value pairs and pass each pair to storage as soon as value
    // has been fully stored locally.
}

void StorageDataSinkJSON::End() {
    // Re-set things needed to keep track of keys and values.
}

void StorageDataSinkJSON::Discard(RawData::Iterator& Start, RawData::Iterator& End)
{
    // Whatever we have, dump and free memory and act as if End called.
}
