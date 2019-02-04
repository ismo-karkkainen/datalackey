//
//  StorageDataSink.cpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 16.5.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//
// Licensed under Universal Permissive License. See License.txt.

#include "StorageDataSink.hpp"


StorageDataSink::StorageDataSink(const StringValueMapper* Mapper)
    : mapper(Mapper)
{ }

StorageDataSink::~StorageDataSink() { }
