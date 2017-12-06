//
//  MessagePassThrough.cpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 25.5.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#include "MessagePassThrough.hpp"
#include "NullValue.hpp"
#include "Notifications.hpp"


MessagePassThrough::MessagePassThrough(
    Output& Out, const SimpleValue* Identifier)
    : out(Out), writer(nullptr), identifier(Identifier)
{
    if (identifier != nullptr) {
        const NullValue* null(dynamic_cast<const NullValue*>(identifier));
        if (null != nullptr)
            identifier = nullptr;
    }
}

MessagePassThrough::~MessagePassThrough() {
    End();
}

const char *const MessagePassThrough::Format() const {
    return nullptr;
}

bool MessagePassThrough::Input(
    RawData::ConstIterator& Start, RawData::ConstIterator& End)
{
    if (identifier == nullptr)
        return true;
    if (writer == nullptr) {
        writer = out.Writable();
        *writer << Array;
        Feed(*writer, *identifier);
    }
    writer->Write(Start, End);
    return true;
}

bool MessagePassThrough::End() {
    if (identifier == nullptr)
        return true;
    *writer << Structure::End;
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
