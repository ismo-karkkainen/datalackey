//
//  OutputItem.cpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 6.12.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#include "OutputItem.hpp"

OutputItem::~OutputItem() { }

OutputItemDiscarder::~OutputItemDiscarder() { }

OutputItem& OutputItemDiscarder::operator<<(Structure S) {
    return *this;
}

OutputItem& OutputItemDiscarder::operator<<(const ValueReference& VR) {
    return *this;
}

void OutputItemDiscarder::Write(
    RawData::ConstIterator Start, RawData::ConstIterator End)
{ }
