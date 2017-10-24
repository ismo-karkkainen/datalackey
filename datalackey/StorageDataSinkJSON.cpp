//
//  StorageDataSinkJSON.cpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 26.5.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#include "StorageDataSinkJSON.hpp"
#include "Notifications.hpp"
#include "json.hpp"

using json = nlohmann::json;


bool StorageDataSinkJSON::pass_to_storage() {
    std:string name;
    try {
        json s = json::parse(key.cbegin(), key.cend());
        if (!s.is_string()) {
            Error(notifications, "identifier", "not-string");
            return false;
        }
        name = s;
    }
    catch (const std::exception& e) {
        Error(notifications, "format");
        return false;
    }
    Label label(name);
    if (mapper != nullptr)
        label = (*mapper)[label];
    storage.Store(label, Format(), value);
    key.clear();
    value.Clear();
    return true;
}

StorageDataSinkJSON::StorageDataSinkJSON(Storage& S,
    Output& ProblemNotifications)
    : storage(S), notifications(ProblemNotifications), part(InHead),
    open_dicts(0), open_arrays(0), in_string(false), escaping(false)
{ }

StorageDataSinkJSON::~StorageDataSinkJSON() {
}

const char *const StorageDataSinkJSON::Format() const {
    return "JSON";
}

bool StorageDataSinkJSON::Input(
    RawData::ConstIterator& Start, RawData::ConstIterator& End)
{
    // Split to key, value pairs and pass each pair to storage as soon as value
    // has been fully stored locally.
    for (RawData::ConstIterator curr = Start; curr != End; ++curr) {
        switch (part) {
        case InHead:
            // No incoming leading white-space, only allow {.
            if (*curr == '{') {
                ++open_dicts;
                part = InKey;
            } else {
                part = BadInput;
                Error(notifications, "format");
                return false;
            }
            break;
        case InKey:
            if (in_string) {
                key.push_back(*curr);
                if (escaping) {
                    escaping = false;
                } else if (*curr == '\\') {
                    escaping = true; // Next character is to be ignored.
                } else if (*curr == '"') {
                    in_string = false; // Ending quote.
                    part = InColon;
                }
                continue;
            }
            // Allow leading white-space before double-quote.
            switch (*curr) {
            case ' ':
            case '\t':
            case '\n':
            case '\r':
                break;
            case '"':
                key.push_back(*curr);
                in_string = true;
                break;
            default:
                part = BadInput;
                Error(notifications, "format");
                return false;
            }
            break;
        case InColon:
            // Allow leading white-space and then a colon.
            switch (*curr) {
            case ' ':
            case '\t':
            case '\n':
            case '\r':
                break;
            case ':':
                part = InValue;
                break;
            default:
                part = BadInput;
                Error(notifications, "format");
                return false;
            }
            break;
        case InValue:
            // Feed characters into value until we get top-level comma.
            if (in_string) {
                if (escaping) {
                    escaping = false;
                } else if (*curr == '"') {
                    in_string = false; // Ending quote.
                } else if (*curr == '\\')
                    escaping = true; // Next character meaning ignored.
                value.Append(*curr); // Accumulate the current value.
                continue;
            }
            switch (*curr) {
            case '[':
                ++open_arrays;
                break;
            case ']':
                if (--open_arrays < 0) {
                    part = BadInput;
                    Error(notifications, "format");
                    return false;
                }
                break;
            case '{':
                ++open_dicts;
                break;
            case '}':
                --open_dicts;
                if (open_dicts == 0) {
                    // We have the last value in the dictionary stored already.
                    if (!pass_to_storage())
                        return false;
                    // Expecting End(), anything else is an error.
                    part = InEnd;
                    continue; // Do not store the separator anywhere.
                } else if (open_dicts < 0) {
                    part = BadInput;
                    Error(notifications, "format");
                    return false;
                }
                break;
            case '"':
                in_string = true;
                break;
            case ' ':
            case '\t':
            case '\n':
            case '\r':
                continue; // Drop whitespace from between elements.
                break;
            case ',':
                if (open_dicts == 1 && open_arrays == 0) {
                    // A comma that separates value from next key.
                    // We have key and value stored already.
                    pass_to_storage();
                    part = InKey;
                    continue; // Do not store the separator anywhere.
                }
                break;
            default:
                break;
            }
            value.Append(*curr); // Accumulate the current value.
            break;
        case InEnd:
        case BadInput:
            return false; // Everything until End is called is suspect.
        }
    }
    return true;
}

bool StorageDataSinkJSON::End() {
    // Re-set things needed to keep track of keys and values.
    part = InHead;
    open_dicts = open_arrays = 0;
    in_string = escaping = false;
    key.clear();
    value.Clear();
    return true;
}

void StorageDataSinkJSON::Discard(
    RawData::ConstIterator& Start, RawData::ConstIterator& End)
{
    // Probably not getting any data.
    value.Clear(true);
    this->End();
}
