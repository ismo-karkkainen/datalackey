//
//  MessagePassThrough.cpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 25.5.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#include "MessagePassThrough.hpp"


MessagePassThrough::MessagePassThrough(Output& Out)
    : out(Out), writer(nullptr)
{ }

MessagePassThrough::~MessagePassThrough() {
    End();
}

const char *const MessagePassThrough::Format() const {
    return nullptr;
}

void MessagePassThrough::Input(RawData::Iterator& Start, RawData::Iterator& End)
{
    if (writer == nullptr)
        writer = out.Writable(true);
    writer->Write(Start, End);
}

void MessagePassThrough::End() {
    delete writer;
    writer = nullptr;
}

void MessagePassThrough::Discard(
    RawData::Iterator& Start, RawData::Iterator& End)
{
    if (writer != nullptr && Start != End)
        writer->Write(Start, End);
    this->End();
}
