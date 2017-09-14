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

bool MessagePassThrough::Input(
    RawData::ConstIterator& Start, RawData::ConstIterator& End)
{
    if (writer == nullptr)
        writer = out.Writable(true);
    writer->Write(Start, End);
    return true;
}

bool MessagePassThrough::End() {
    delete writer;
    writer = nullptr;
    return true;
}

void MessagePassThrough::Discard(
    RawData::ConstIterator& Start, RawData::ConstIterator& End)
{
    // If writing, the error is someone else's problem.
    if (writer != nullptr && Start != End)
        writer->Write(Start, End);
    this->End();
}
