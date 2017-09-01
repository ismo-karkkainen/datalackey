//
//  StorageDataSinkJSON.cpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 26.5.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#include "StorageDataSinkJSON.hpp"


void StorageDataSinkJSON::pass_to_storage() {
    storage.Store(std::string(key.cbegin(), key.cend()), Format(), value);
    key.clear();
    value.Clear();
}

StorageDataSinkJSON::StorageDataSinkJSON(Storage& S)
    : storage(S), part(InHead), open_dicts(0), open_arrays(0),
    in_string(false), escaping(false)
{ }

StorageDataSinkJSON::~StorageDataSinkJSON() {
}

const char *const StorageDataSinkJSON::Format() const {
    return "JSON";
}

void StorageDataSinkJSON::Input(
    RawData::Iterator& Start, RawData::Iterator& End)
{
    // Split to key, value pairs and pass each pair to storage as soon as value
    // has been fully stored locally.
    for (RawData::Iterator curr = Start; curr != End; ++curr) {
        switch (part) {
        case InHead:
            // No incoming leading white-space, only allow {.
            if (*curr == '{') {
                if (open_dicts)
                    part = BadInput;
                ++open_dicts;
                part = InKey;
            } else {
                part = BadInput;
                return;
            }
            break;
        case InKey:
            if (in_string) {
                if (escaping) {
                    escaping = false;
                    key.push_back(*curr);
                } else if (*curr == '"') {
                    in_string = false; // Ending quote.
                    part = InColon;
                    continue;
                } else if (*curr == '\\')
                    escaping = true; // Next character is to be skipped.
                else
                    key.push_back(*curr);
                if (in_string)
                    continue; // No need for other checks.
            }
            // Allow leading white-space before double-quote.
            switch (*curr) {
            case ' ':
            case '\t':
            case '\n':
            case '\r':
                break;
            case '"':
                in_string = true;
                break;
            default:
                part = BadInput;
                return;
            }
            break;
        case InColon:
            // Allow leading white-pace and then a colon.
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
                return;
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
                --open_arrays;
                break;
            case '{':
                ++open_dicts;
                break;
            case '}':
                --open_dicts;
                if (open_dicts == 0) {
                    // We have the last value in the dictionary stored already.
                    pass_to_storage();
                    // Expecting End(), anything else is an error.
                    part = InEnd;
                    continue; // Do not store the separator anywhere.
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
            return; // Everything until End is called is suspect.
        }
    }
}

void StorageDataSinkJSON::End() {
    // Re-set things needed to keep track of keys and values.
    part = InHead;
    open_dicts = open_arrays = 0;
    in_string = escaping = false;
    key.clear();
    value.Clear();
}

void StorageDataSinkJSON::Discard(
    RawData::Iterator& Start, RawData::Iterator& End)
{
    // Whatever we have, dump and free memory. No valid input coming.
    part = BadInput;
    key.clear();
    value.Clear(true);
}
