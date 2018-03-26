//
//  StorageDataSinkJSON.cpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 26.5.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#include "StorageDataSinkJSON.hpp"
#include "Messages.hpp"
#include <nlohmann/json.hpp>

using json = nlohmann::json;


bool StorageDataSinkJSON::start_item() {
    std::string name;
    try {
        json s = json::parse(key.cbegin(), key.cend());
        key.resize(0);
        if (!s.is_string()) {
            Message(notifications, identifier,
                "error", "identifier", "not-string");
            return false;
        }
        name = s;
    }
    catch (const std::exception& e) {
        return error_format();
    }
    StringValue label(name);
    if (renamer != nullptr)
        label = (*renamer)[label];
    ignoring_item = label.String().empty();
    if (!ignoring_item) {
        group->StartItem(label);
        value.Clear();
    }
    return true;
}

void StorageDataSinkJSON::pass_item() {
    if (ignoring_item)
        return;
    RawData::ConstIterator begin(value.CBegin());
    RawData::ConstIterator end(value.CEnd());
    group->Append(begin, end);
    value.Clear();
}

void StorageDataSinkJSON::end_item() {
    if (ignoring_item)
        return;
    pass_item();
    group->Finish();
}

void StorageDataSinkJSON::end_group() {
    if (open_dicts == 0 && group != nullptr && part != BadInput) {
        std::vector<std::string> labels = group->Labels();
        if (notifications.ControllerOutput() != nullptr) {
            // Ensure that the pointer is still valid.
            std::lock_guard<std::mutex> lg(DataNotifiedOutputs.Mutex());
            for (Output* out : DataNotifiedOutputs.Outputs())
                if (out == notifications.ControllerOutput())
                    ListMessage(*out, *identifier, "data", "stored", labels);
        }
        storage.Add(*group, notifications.ControllerOutput());
    }
    delete group;
    group = nullptr;
}

bool StorageDataSinkJSON::error_format() const {
    Message(notifications, identifier, "error", "format");
    return false;
}

StorageDataSinkJSON::StorageDataSinkJSON(Storage& S,
    const SimpleValue* Id, Output& ProblemNotifications,
    const StringValueMapper* Renamer)
    : storage(S), group(nullptr), identifier(nullptr),
    notifications(ProblemNotifications), renamer(Renamer),
    part(InHead), open_dicts(0), open_arrays(0),
    in_string(false), escaping(false)
{
    if (Id != nullptr)
        identifier = Id->Clone();
}

StorageDataSinkJSON::~StorageDataSinkJSON() {
    delete identifier;
    delete group;
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
                group = new DataGroup(Format());
                part = InKey;
            } else {
                part = BadInput;
                return error_format();
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
                    if (!start_item()) {
                        part = BadInput;
                        return error_format();
                    }
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
                return error_format();
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
                return error_format();
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
                break;
            }
            switch (*curr) {
            case '[':
                ++open_arrays;
                break;
            case ']':
                if (--open_arrays < 0) {
                    part = BadInput;
                    return error_format();
                }
                break;
            case '{':
                ++open_dicts;
                break;
            case '}':
                --open_dicts;
                if (open_dicts == 0) {
                    end_item();
                    end_group();
                    // Expecting End(), anything else is an error.
                    part = InEnd;
                    continue; // Do not store the separator anywhere.
                } else if (open_dicts < 0) {
                    part = BadInput;
                    return error_format();
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
                    end_item();
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
        if (value.Size() >= 16384)
            pass_item();
    }
    return true;
}

bool StorageDataSinkJSON::End() {
    end_group();
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
