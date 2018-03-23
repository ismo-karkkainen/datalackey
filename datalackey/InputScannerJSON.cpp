//
//  InputScannerJSON.cpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 4.9.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#include "InputScannerJSON.hpp"
#include "Messages.hpp"
#include <tuple>
#include <cassert>
#include <ctype.h>


static void reset_msg(Output& notifications, const SimpleValue* identifier) {
    Message(notifications, identifier, "channel", "reset");
}

std::tuple<InputScanner::Recipient, RawData::ConstIterator, RawData::ConstIterator>
InputScannerJSON::scan_input(InputScanner::Recipient Previous,
    RawData::ConstIterator RangeBegin, RawData::ConstIterator RangeEnd)
{
    // Stream is in bad state, scans for zero byte allowed to reset it.
    if (Previous == InputScanner::DiscardRetroactively) {
        for (RawData::ConstIterator curr = RangeBegin; curr != RangeEnd; ++curr)
        {
            if (*curr)
                continue;
            in_string = escaping = false;
            open_something = 0;
            reset_msg(notifications, identifier);
            return std::make_tuple(InputScanner::Reset, RangeBegin, ++curr);
        }
        return std::make_tuple(
            InputScanner::DiscardRetroactively, RangeBegin, RangeEnd);
    }
    // Performs discarding of meaningless space inside array or dictionary.
    if (open_something > 0 && !in_string && isspace(*RangeBegin)) {
        RawData::ConstIterator curr = RangeBegin + 1;
        while (curr != RangeEnd && isspace(*curr))
            ++curr;
        return std::make_tuple(InputScanner::SpaceDiscard, RangeBegin, curr);
    }
    // Could arrange this to first check for state and then loop inside it.
    for (RawData::ConstIterator curr = RangeBegin; curr != RangeEnd; ++curr) {
        if (*curr == 0) {
            // A resetting zero byte interrupts everything we are doing.
            open_something = 0;
            in_string = escaping = false;
            if (Previous != InputScanner::Reset)
                reset_msg(notifications, identifier);
            return std::make_tuple(InputScanner::Reset, RangeBegin, ++curr);
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
            } else
                bad_stream = true;
            break;
        default:
            if (isspace(*curr)) { // Whitespace outside strings.
                if (open_something > 0) // Message/Data up to this.
                    return std::make_tuple(Previous, RangeBegin, curr);
            } else
                bad_stream = open_something <= 0;
            break;
        }
        if (bad_stream) {
            // This is used by main program (no identifier) or process started
            // via run command. If more cases appear, the Class parameter has
            // to become a separate parameter.
            if (identifier != nullptr)
                ::Message(notifications, identifier, "run", "error", "format");
            else
                ::Message(notifications, "error", "format");
            return std::make_tuple(InputScanner::DiscardRetroactively,
                RangeBegin, ++curr);
        }
    }
    return std::make_tuple(Previous, RangeBegin, RangeEnd); // Used all.
}

InputScannerJSON::InputScannerJSON(InputChannel& IC, MessageHandler& MH,
    StorageDataSink& SDS, Output& ProblemNotifications, const SimpleValue* Id)
    : InputScanner(IC, MH, SDS), open_something(0),
    in_string(false), escaping(false), notifications(ProblemNotifications),
    identifier(Id)
{
    assert(MH.Format() == nullptr || strcmp(Format(), MH.Format()) == 0);
    assert(strcmp(Format(), SDS.Format()) == 0);
}

InputScannerJSON::~InputScannerJSON() {
}

const char* const InputScannerJSON::Format() const {
    return "JSON";
}
