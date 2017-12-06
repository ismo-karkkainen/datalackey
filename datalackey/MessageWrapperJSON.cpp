//
//  MessageWrapperJSON.cpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 3.12.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#include "MessageWrapperJSON.hpp"
#include "NullValue.hpp"
#include "Notifications.hpp"
#include "Value_t.hpp"


MessageWrapperJSON::MessageWrapperJSON(
    Output& Out, const SimpleValue* Identifier)
    : out(Out), writer(nullptr), identifier(Identifier)
{
    const NullValue* null(dynamic_cast<const NullValue*>(identifier));
    if (null != nullptr)
        identifier = nullptr;
}

MessageWrapperJSON::~MessageWrapperJSON() {
    End();
}

const char *const MessageWrapperJSON::Format() const {
    return "JSON";
}

bool MessageWrapperJSON::Input(
    RawData::ConstIterator& Start, RawData::ConstIterator& End)
{
    if (identifier == nullptr)
        return true;
    if (writer == nullptr) {
        writer = out.Writable();
        *writer << Array;
        Feed(*writer, *identifier);
    }
    // Writes the bytes as an array.
    *writer << Array;
    while (Start != End)
        *writer << ValueRef<unsigned>(static_cast<unsigned char>(*Start++));
    *writer << Structure::End;
    return true;
}

bool MessageWrapperJSON::End() {
    if (identifier == nullptr)
        return true;
    *writer << Structure::End;
    delete writer;
    writer = nullptr;
    return true;
}

void MessageWrapperJSON::Discard(
    RawData::ConstIterator& Start, RawData::ConstIterator& End)
{
    // Given the scanned input is not structured, this should not happen.
    if (writer != nullptr && Start != End)
        Input(Start, End);
    this->End();
}
