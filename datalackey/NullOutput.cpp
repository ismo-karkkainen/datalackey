//
//  NullOutput.cpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 29.1.18.
//  Copyright © 2018 Ismo Kärkkäinen. All rights reserved.
//
// Licensed under Universal Permissive License. See License.txt.

#include "NullOutput.hpp"


NullOutput::NullOutput()
    : closed(false)
{ }

NullOutput::~NullOutput() { }

size_t NullOutput::Write(
    RawData::ConstIterator& Start, RawData::ConstIterator& End)
{
    if (closed)
        return 0;
    return End - Start;
}

void NullOutput::Flush() { }

void NullOutput::Close() {
    closed = true;
}

bool NullOutput::Failed() const {
    return false;
}

bool NullOutput::Closed() const {
    return closed;
}
