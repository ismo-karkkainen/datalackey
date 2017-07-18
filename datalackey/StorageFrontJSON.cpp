//
//  StorageFrontJSON.cpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 26.5.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#include "StorageFrontJSON.hpp"


StorageFrontJSON::StorageFrontJSON(Storage& S)
    : storage(S)
{ }

StorageFrontJSON::~StorageFrontJSON() {
}

const char *const StorageFrontJSON::Format() const {
    return "JSON";
}

void StorageFrontJSON::Input(Iterator& Start, Iterator& End)
{
    // Split to key, value pairs and pass each pair to storage as soon as value
    // has been fully stored locally.
}

void StorageFrontJSON::End() {
    // Re-set things needed to keep track of keys and values.
}

void StorageFrontJSON::Discard(Iterator& Start, Iterator& End)
{
    // Whatever we have, dump and free memory and act as if End called.
}
