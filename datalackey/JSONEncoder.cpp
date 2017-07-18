//
//  JSONEncoder.cpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 9.5.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#include "JSONEncoder.hpp"


void JSONEncoder::add_separator(std::vector<char>& Buffer) {
    if (open.empty())
        return;
    if (open.top() == Array) {
        if (comma.top())
            Buffer.push_back(',');
        else {
            comma.pop();
            comma.push(true);
        }
    } else {
        if (comma.top())
            Buffer.push_back(',');
        else
            Buffer.push_back(':');
        bool old = comma.top();
        comma.pop();
        comma.push(!old);
    }
}

JSONEncoder::JSONEncoder() {
}

JSONEncoder::~JSONEncoder() {
}

bool JSONEncoder::Encode(std::vector<char>& Buffer, Structure S) {
    switch (S) {
    case Array:
        add_separator(Buffer);
        Buffer.push_back('[');
        open.push(S);
        comma.push(false);
        break;
    case Dictionary:
        add_separator(Buffer);
        Buffer.push_back('{');
        open.push(S);
        comma.push(false);
        break;
    case End:
        if (open.top() == Array)
            Buffer.push_back(']');
        else if (open.top() == Dictionary) {
            //if (!comma.top()) // Would be an error. Exception? What?
            Buffer.push_back('}');
        }
        open.pop();
        comma.pop();
        break;
    }
    return true;
}

bool JSONEncoder::Encode(std::vector<char>& Buffer, const ValueReference& VR) {
    add_separator(Buffer);
    if (VR.IsChar() || VR.IsString()) {
        Buffer.push_back('"');
        std::string s = VR.String();
        for (auto c: s) {
            if (c == '"' || c == '\\')
                Buffer.push_back('\\');
            Buffer.push_back(c);
        }
        Buffer.push_back('"');
    } else {
        std::string s = VR.String();
        for (auto c: s)
            Buffer.push_back(c);
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
