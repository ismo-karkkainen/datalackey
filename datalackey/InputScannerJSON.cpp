//
//  InputScannerJSON.cpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 4.9.17.
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
    if (open_something == 0) {
        // Start something new, possibly.
        switch (*RangeBegin) {
        case '[':
            next = InputScanner::Message;
            break;
        case '{':
            next = InputScanner::Data;
            break;
        default:
            next = InputScanner::Discard; // Top-level white-space. Or error.
            break;
        }
    }
    // Could arrange this to first check for state and then loop inside it.
    for (RawData::Iterator curr = RangeBegin; curr != RangeEnd; ++curr) {
        if (open_something == 0 && curr != RangeBegin && next != InputScanner::Discard)
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
        case '{':
            if (open_something == 0 && next == InputScanner::Discard)
                return std::make_tuple(
                    InputScanner::Discard, RangeBegin, curr);
                // Item starts, curr will be first character next time around.
            ++open_something;
            break;
        case ']':
            --open_something;
            if (open_something == 0 && next == InputScanner::Message)
                next = InputScanner::MessageEnd;
            else if (open_something < 0) {
                bad_stream = true;
                return std::make_tuple(InputScanner::DiscardRetroactively,
                    RangeBegin, RangeEnd);
            }
            break;
        case '}':
            --open_something;
            if (open_something == 0 && next == Data)
                next = InputScanner::DataEnd;
            else if (open_something < 0) {
                bad_stream = true;
                return std::make_tuple(InputScanner::DiscardRetroactively,
                    RangeBegin, RangeEnd);
            }
            break;
        case '"':
            if (0 < open_something) {
                in_string = true;
                break;
            }
            bad_stream = true;
            return std::make_tuple(InputScanner::DiscardRetroactively,
                RangeBegin, RangeEnd);
        case ' ':
        case '\t':
        case '\n':
        case '\r':
            break; // Ignore whitespace always.
        default:
            if (0 < open_something)
                break;
            // Non-whitespace between items not allowed.
            bad_stream = true;
            return std::make_tuple(InputScanner::DiscardRetroactively,
                RangeBegin, RangeEnd);
        }
    }
    return std::tie(next, RangeBegin, RangeEnd); // Used all.
}

InputScannerJSON::InputScannerJSON(
    InputChannel& IC, MessageHandler& MH, StorageDataSink& SDS)
    : InputScanner(IC, MH, SDS), open_something(0),
    in_string(false), escaping(false), bad_stream(false)
{
    assert(MH.Format() == nullptr || strcmp(Format(), MH.Format()) == 0);
    assert(strcmp(Format(), SDS.Format()) == 0);
}

InputScannerJSON::~InputScannerJSON() {
}

const char* const InputScannerJSON::Format() const {
    return "JSON";
}
