//
//  InputScannerJSON.cpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 30.5.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#include "InputScannerJSON.hpp"
#include <tuple>
#include <cassert>


std::tuple<InputScanner::Recipient, RawData::Iterator, RawData::Iterator>
InputScannerJSON::scan_input(InputScanner::Recipient Previous,
    RawData::Iterator RangeBegin, RawData::Iterator RangeEnd)
{
    if (bad_stream)
        return std::make_tuple(InputScanner::Discard, RangeBegin, RangeEnd);
    InputScanner::Recipient next = Previous;
    if (open.empty()) {
        // Start something new, possibly.
        switch (*RangeBegin) {
        case '[':
            next = InputScanner::Message;
            break;
        case '{':
            next = InputScanner::Data;
            break;
        default:
            next = InputScanner::Discard;
            break;
        }
    }
    // Could arrange this to first check for state and then loop inside it.
    for (RawData::Iterator curr = RangeBegin; curr != RangeEnd; ++curr) {
        if (open.empty() && curr != RangeBegin && next != InputScanner::Discard)
        {
            // Something ended and we just skipped its end.
            return std::make_tuple(next, RangeBegin, curr);
        }
        if (in_string) {
            if (escaping)
                escaping = false; // Skips current character doing nothing.
            else if (*curr == '"')
                in_string = false; // Ending quote.
            else if (*curr == '\\')
                escaping = true; // Next character meaning ignored.
            continue;
        }
        switch (*curr) {
        case '[':
            if (open.empty() && next == InputScanner::Discard)
                return std::make_tuple(
                    InputScanner::Discard, RangeBegin, curr);
                // curr will be first character next time around.
            open.push(Array);
            break;
        case ']':
            if (open.top() != Array) {
                bad_stream = true;
                // Ought to construct the closing array.
                return std::make_tuple(InputScanner::DiscardRetroactively,
                    RangeBegin, RangeEnd);
            }
            open.pop();
            if (open.empty() && next == InputScanner::Message)
                next = InputScanner::MessageEnd;
            break;
        case '{':
            if (open.empty() && next == InputScanner::Discard)
                return std::make_tuple(
                    InputScanner::Discard, RangeBegin, curr);
            open.push(Dictionary);
            break;
        case '}':
            if (open.top() != Dictionary) {
                bad_stream = true;
                return std::make_tuple(InputScanner::DiscardRetroactively,
                    RangeBegin, RangeEnd);
            }
            open.pop();
            if (open.empty() && next == Data)
                next = InputScanner::DataEnd;
            break;
        case '"':
            if (open.empty()) {
                bad_stream = true;
                return std::make_tuple(InputScanner::DiscardRetroactively,
                    RangeBegin, RangeEnd);
            }
            in_string = true;
            break;
        case ' ':
        case '\t':
        case '\n':
        case '\r':
            break; // Ignore whitespace always.
        default:
            if (open.empty()) {
                // Non-whitespace between items not allowed.
                bad_stream = true;
                return std::make_tuple(InputScanner::DiscardRetroactively,
                    RangeBegin, RangeEnd);
            }
            break;
        }
    }
    return std::tie(next, RangeBegin, RangeEnd); // Used all.
}

InputScannerJSON::InputScannerJSON(
    InputChannel& IC, MessageHandler& MH, StorageDataSink& SDS)
    : InputScanner(IC, MH, SDS), in_string(false), escaping(false),
    bad_stream(false)
{
    assert(MH.Format() == nullptr || strcmp(Format(), MH.Format()) == 0);
    assert(strcmp(Format(), SDS.Format()) == 0);
}

InputScannerJSON::~InputScannerJSON() {
}

const char* const InputScannerJSON::Format() const {
    return "JSON";
}
