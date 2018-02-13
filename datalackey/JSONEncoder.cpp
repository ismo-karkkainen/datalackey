//
//  JSONEncoder.cpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 9.5.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#include "JSONEncoder.hpp"
#include <nlohmann/json.hpp>

using json = nlohmann::json;


void JSONEncoder::add_separator(RawData& Buffer) {
    if (open.empty())
        return;
    if (open.top() == Array) {
        if (counter.top())
            Buffer.Append(',');
        else {
            counter.pop();
            counter.push(1);
        }
    } else {
        if (counter.top() == 1) {
            Buffer.Append(':');
            counter.pop();
            counter.push(2);
        } else {
            if (counter.top() == 2)
                Buffer.Append(',');
            counter.pop();
            counter.push(1);
        }
    }
}

JSONEncoder::JSONEncoder() {
}

JSONEncoder::~JSONEncoder() {
}

bool JSONEncoder::Encode(RawData& Buffer, Structure S) {
    switch (S) {
    case Array:
        add_separator(Buffer);
        Buffer.Append('[');
        open.push(S);
        counter.push(0);
        break;
    case Dictionary:
        add_separator(Buffer);
        Buffer.Append('{');
        open.push(S);
        counter.push(0);
        break;
    case End:
        if (open.top() == Array)
            Buffer.Append(']');
        else if (open.top() == Dictionary) {
            // An opportunity to check for missing value in key:value.
            Buffer.Append('}');
        }
        open.pop();
        counter.pop();
        if (open.empty())
            Buffer.Append('\n');
        break;
    case True:
        add_separator(Buffer);
        Buffer.Append("true");
        break;
    case False:
        add_separator(Buffer);
        Buffer.Append("false");
        break;
    case Null:
        add_separator(Buffer);
        Buffer.Append("null");
        break;
    case RawItem:
        add_separator(Buffer);
        break;
    }
    return true;
}

bool JSONEncoder::Encode(RawData& Buffer, const ValueReference& VR) {
    add_separator(Buffer);
    if (VR.IsChar() || VR.IsString()) {
        std::string encoded(json(VR.String()).dump());
        Buffer.Append(encoded.cbegin(), encoded.cend());
    } else {
        Buffer.Append(VR.String().c_str());
    }
    return true;
}

Encoder* JSONEncoder::Clone() const {
    return new JSONEncoder();
}

bool JSONEncoder::EncodeOutputsDirectly() const {
    return true;
}

const char *const JSONEncoder::Format() const {
    return "JSON";
}
