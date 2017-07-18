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
        Iterator begin = IS->buffer.begin();
        Iterator end = begin;
        do {
            InputScanner::Recipient recipient;
            std::tie(recipient, begin, end) =
                IS->scan_input(previous, begin, IS->buffer.end());
            switch (recipient) {
            case InputScanner::Discard:
                break;
            case InputScanner::DiscardRetroactively:
                if (previous == InputScanner::Data) {
                    IS->data_sink.Discard(begin, end);
                } else if (previous == InputScanner::Message) {
                    IS->message_sink.Discard(begin, end);
                }
                end = IS->buffer.end();
                break;
            case InputScanner::Data:
                IS->data_sink.Input(begin, end);
                break;
            case InputScanner::DataEnd:
                IS->data_sink.Input(begin, end);
                IS->data_sink.End();
                break;
            case InputScanner::Message:
                IS->message_sink.Input(begin, end);
                break;
            case InputScanner::MessageEnd:
                IS->message_sink.Input(begin, end);
                IS->message_sink.End();
                break;
            }
            begin = end;
            previous = recipient;
        } while (end != IS->buffer.end());
        IS->buffer.resize(0);
    }
}


InputScanner::InputScanner(InputChannel& IC, MessageHandler& MH, StorageFront& SF)
    : channel(IC), message_sink(MH), data_sink(SF), worker(nullptr)
{
}

InputScanner::~InputScanner() {
    if (worker != nullptr) {
        worker->join();
        delete worker;
    }
}

void InputScanner::Scan() {
    if (worker != nullptr)
        return;
    worker = new std::thread(&input_scanner, this);
}
