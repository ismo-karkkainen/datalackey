//
//  InputScanner.cpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 22.5.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#include "InputScanner.hpp"
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
        RawData::Iterator begin = IS->buffer.Begin();
        RawData::Iterator end = begin;
        do {
            bool bad = false;
            InputScanner::Recipient recipient;
            std::tie(recipient, begin, end) =
                IS->scan_input(previous, begin, IS->buffer.End());
            switch (recipient) {
            case InputScanner::Discard:
                break; // Throw blank data away.
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
            previous = recipient;
        } while (end != IS->buffer.End());
        IS->buffer.Clear();
    }
}


InputScanner::InputScanner(InputChannel& IC, MessageHandler& MH, StorageDataSink& SDS)
    : channel(IC), message_sink(MH), data_sink(SDS), worker(nullptr)
{
}

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
