//
//  NullOutput.cpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 29.1.18.
//  Copyright © 2018 Ismo Kärkkäinen. All rights reserved.
//

#include "NullOutput.hpp"


NullOutput::~NullOutput() { }

size_t NullOutput::Write(
    RawData::ConstIterator& Start, RawData::ConstIterator& End)
{
    return End - Start;
}

void NullOutput::Flush() { }

void NullOutput::Close() { }

bool NullOutput::Failed() const {
    return false;
}
