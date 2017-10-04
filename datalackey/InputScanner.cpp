//
//  InputScanner.cpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 22.5.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#include "InputScanner.hpp"
#include "Structure.hpp"
#include "Value_t.hpp"
#include <cstring>
#include <ctime>
#include <cassert>

void input_scanner(InputScanner* IS) {
    InputScanner::Recipient previous = InputScanner::Discard;
    while (!IS->Ended()) {
        // Check channel.
        if (!IS->channel.Read(IS->buffer)) { // Nothing new?
            struct timespec ts;
            ts.tv_sec = 0;
            ts.tv_nsec = 10000000;
            nanosleep(&ts, nullptr);
            continue;
        }
        // Use all data piece by piece and keep track of used part.
        RawData::ConstIterator begin = IS->buffer.CBegin();
        RawData::ConstIterator end = begin;
        do {
            bool bad = false;
            InputScanner::Recipient recipient;
            std::tie(recipient, begin, end) =
                IS->scan_input(previous, begin, IS->buffer.CEnd());
            switch (recipient) {
            case InputScanner::Discard:
                break; // Throw blank data away.
            case InputScanner::Reset:
                // Same action as with next.
            case InputScanner::DiscardRetroactively:
                // If passing data in, tell to discard it.
                if (previous == InputScanner::Data)
                    IS->data_sink.Discard(begin, end);
                else if (previous == InputScanner::Message)
                    IS->message_sink.Discard(begin, end);
                break;
            case InputScanner::Data:
                bad = !IS->data_sink.Input(begin, end);
                break;
            case InputScanner::DataEnd:
                bad = !IS->data_sink.Input(begin, end);
                bad = !IS->data_sink.End() || bad;
                recipient = InputScanner::Discard;
                break;
            case InputScanner::Message:
                bad = !IS->message_sink.Input(begin, end);
                break;
            case InputScanner::MessageEnd:
                bad = !IS->message_sink.Input(begin, end);
                bad = !IS->message_sink.End() || bad;
                recipient = InputScanner::Discard;
                break;
            }
            if (bad)
                recipient = InputScanner::DiscardRetroactively;
            begin = end;
            if (!bad && previous != recipient) {
                if (recipient == InputScanner::DiscardRetroactively) {
                    OutputItem* writer = IS->notifications.Writable();
                    *writer << Array
                        << ValueRef<std::string>("error")
                        << ValueRef<std::string>("format")
                        << End;
                    delete writer;
                } else if (recipient == InputScanner::Reset) {
                    OutputItem* writer = IS->notifications.Writable();
                    *writer << Array
                        << ValueRef<std::string>("note")
                        << ValueRef<std::string>("reset")
                        << End;
                    delete writer;
                }
            }
            previous = recipient;
        } while (end != IS->buffer.CEnd());
        IS->buffer.Clear();
    }
}


InputScanner::InputScanner(InputChannel& IC, MessageHandler& MH,
    StorageDataSink& SDS, Output& ProblemNotifications)
    : channel(IC), message_sink(MH), data_sink(SDS), worker(nullptr),
    notifications(ProblemNotifications)
{ }

InputScanner::~InputScanner() {
    assert(worker == nullptr);
}

void InputScanner::Scan() {
    if (worker != nullptr)
        return;
    worker = new std::thread(&input_scanner, this);
}

void InputScanner::FinishScan() {
    worker->join();
    delete worker;
    worker = nullptr;
}
