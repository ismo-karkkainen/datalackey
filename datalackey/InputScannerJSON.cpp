//
//  InputScannerJSON.cpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 4.9.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#include "InputScannerJSON.hpp"
#include "Notifications.hpp"
#include <tuple>
#include <cassert>


std::tuple<InputScanner::Recipient, RawData::ConstIterator, RawData::ConstIterator>
InputScannerJSON::scan_input(InputScanner::Recipient Previous,
    RawData::ConstIterator RangeBegin, RawData::ConstIterator RangeEnd)
{
    if (Previous == InputScanner::DiscardRetroactively) {
        // Allow zero byte in the stream to reset it.
        for (RawData::ConstIterator curr = RangeBegin; curr != RangeEnd; ++curr)
        {
            if (*curr)
                continue;
            in_string = escaping = false;
            open_something = 0;
            Note(notifications, "reset");
            return std::make_tuple(InputScanner::Reset, RangeBegin, ++curr);
        }
        return std::make_tuple(
            InputScanner::DiscardRetroactively, RangeBegin, RangeEnd);
    }
    // Could arrange this to first check for state and then loop inside it.
    for (RawData::ConstIterator curr = RangeBegin; curr != RangeEnd; ++curr) {
        if (*curr == 0) {
            // A resetting zero byte interrupts everything we are doing.
            open_something = 0;
            in_string = escaping = false;
            if (Previous != InputScanner::Reset)
                Note(notifications, "reset");
            return std::make_tuple(InputScanner::Reset,
                RangeBegin, ++curr);
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
        bool bad_stream = false;
        switch (*curr) {
        case '[':
        case '{':
            if (open_something == 0) {
                // Discard only when something to discard between items.
                if (Previous == InputScanner::Discard && curr != RangeBegin)
                    return std::make_tuple(
                        InputScanner::Discard, RangeBegin, curr);
                Previous =
                    (*curr == '[') ? InputScanner::Message : InputScanner::Data;
            }
            ++open_something;
            break;
        case ']':
        case '}':
            --open_something;
            if (open_something == 0) {
                if (Previous == InputScanner::Data)
                    return std::make_tuple(
                        InputScanner::DataEnd, RangeBegin, ++curr);
                if (Previous == InputScanner::Message)
                    return std::make_tuple(
                        InputScanner::MessageEnd, RangeBegin, ++curr);
            }
            bad_stream = open_something <= 0;
            break;
        case '"':
            if (0 < open_something) {
                // String only allowed inside array or dictionary.
                in_string = true;
                break;
            }
            bad_stream = true;
            break;
        case ' ':
        case '\t':
        case '\n':
        case '\r':
            break; // Whitespace outside strings.
        default:
            bad_stream = open_something <= 0;
            break;
        }
        if (bad_stream) {
            Error(notifications, "format");
            return std::make_tuple(InputScanner::DiscardRetroactively,
                RangeBegin, ++curr);
        }
    }
    return std::make_tuple(Previous, RangeBegin, RangeEnd); // Used all.
}

InputScannerJSON::InputScannerJSON(InputChannel& IC, MessageHandler& MH,
    StorageDataSink& SDS, Output& ProblemNotifications)
    : InputScanner(IC, MH, SDS), open_something(0),
    in_string(false), escaping(false), notifications(ProblemNotifications)
{
    assert(MH.Format() == nullptr || strcmp(Format(), MH.Format()) == 0);
    assert(strcmp(Format(), SDS.Format()) == 0);
}

InputScannerJSON::~InputScannerJSON() {
}

const char* const InputScannerJSON::Format() const {
    return "JSON";
}
