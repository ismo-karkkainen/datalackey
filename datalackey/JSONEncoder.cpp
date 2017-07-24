//
//  JSONEncoder.cpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 9.5.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#include "JSONEncoder.hpp"


void JSONEncoder::add_separator(RawData& Buffer) {
    if (open.empty())
        return;
    if (open.top() == Array) {
        if (comma.top())
            Buffer.Append(',');
        else {
            comma.pop();
            comma.push(true);
        }
    } else {
        if (comma.top())
            Buffer.Append(',');
        else
            Buffer.Append(':');
        bool old = comma.top();
        comma.pop();
        comma.push(!old);
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
        comma.push(false);
        break;
    case Dictionary:
        add_separator(Buffer);
        Buffer.Append('{');
        open.push(S);
        comma.push(false);
        break;
    case End:
        if (open.top() == Array)
            Buffer.Append(']');
        else if (open.top() == Dictionary) {
            //if (!comma.top()) // Would be an error. Exception? What?
            Buffer.Append('}');
        }
        open.pop();
        comma.pop();
        break;
    }
    return true;
}

bool JSONEncoder::Encode(RawData& Buffer, const ValueReference& VR) {
    add_separator(Buffer);
    if (VR.IsChar() || VR.IsString()) {
        Buffer.Append('"');
        std::string s = VR.String();
        for (auto c: s) {
            if (c == '"' || c == '\\')
                Buffer.Append('\\');
            Buffer.Append(c);
        }
        Buffer.Append('"');
    } else {
        std::string s = VR.String();
        for (auto c: s)
            Buffer.Append(c);
    }
    return true;
}

Encoder* JSONEncoder::CreateSame() const {
    return new JSONEncoder();
}

bool JSONEncoder::EncodeOutputsDirectly() const {
    return true;
}

const char *const JSONEncoder::Format() const {
    return "JSON";
}
