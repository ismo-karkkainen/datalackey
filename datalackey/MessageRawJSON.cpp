//
//  MessageRawJSON.cpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 3.12.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#include "MessageRawJSON.hpp"
#include "NullValue.hpp"
#include "Messages.hpp"
#include "Number_t.hpp"
#include <memory>


MessageRawJSON::MessageRawJSON(
    Output& Out, const SimpleValue& Identifier)
    : out(Out), writer(nullptr), identifier(Identifier)
{ }

MessageRawJSON::~MessageRawJSON() {
    End();
}

const char *const MessageRawJSON::Format() const {
    return "JSON";
}

bool MessageRawJSON::Input(
    RawData::ConstIterator& Start, RawData::ConstIterator& End)
{
    if (writer == nullptr) {
        writer = out.Writable(IsNullValue(&identifier));
        *writer << Array;
        Feed(*writer, identifier);
    }
    // Writes the bytes as an extension to the existing array.
    while (Start != End)
        *writer << NumberRef<unsigned>(static_cast<unsigned char>(*Start++));
    return true;
}

bool MessageRawJSON::End() {
    if (writer != nullptr) {
        *writer << Structure::End;
        delete writer;
        writer = nullptr;
    }
    return true;
}

void MessageRawJSON::Discard(
    RawData::ConstIterator& Start, RawData::ConstIterator& End)
{
    // Given the scanned input is not structured, this should not happen.
    if (writer != nullptr && Start != End)
        Input(Start, End);
    this->End();
}
