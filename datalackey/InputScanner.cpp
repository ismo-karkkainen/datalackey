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
    InputScanner::Recipient previous = Discard;
    while (!IS->Ended()) {
        // Check channel.
        size_t scanned = IS->buffer.size(); // Everything always scanned.
        if (!IS->channel.Read(IS->buffer)) { // Nothing new?
            struct timespec ts;
            ts.tv_sec = 0;
            ts.tv_nsec = 10000000;
            nanosleep(&ts, nullptr);
            continue;
        }
        // Skip scanned old part.
        InputScanner::Iterator first = IS->buffer.begin() + scanned;
        bool waiting = false;
        do {
            InputScanner::Recipient recipient;
            InputScanner::Iterator begin;
            InputScanner::Iterator end;
            std::tie(recipient, begin, end) =
                IS->scan_input(first, IS->buffer.end());
            if (recipient != previous) {
                switch (previous) {
                case Data:
                    IS->data_sink.End();
                    break;
                case Message:
                    IS->message_sink.End();
                    break;
                }
                previous = recipient;
            }
            switch (recipient) {
            case Discard:
                first = end;
                break;
            case More:
                waiting = true;
                break;
            case Data:
                first = IS->data_sink.Input(begin, end);
                waiting = first != end;
                break;
            case Message:
                first = IS->message_sink.Input(begin, end);
                waiting = first != end;
                break;
            }
        } while (first != IS->buffer.end() || !waiting);
        // This avoids having to store partial items in sinks.
        if (first != IS->buffer.end()) {
            IS->buffer.erase(IS->buffer.begin(), first);
        } else {
            IS->buffer.resize(0);
        }
    }
}


InputScanner::InputScanner(InputChannel& IC, MessageHandler& MH, StorageFront& SF)
    : channel(IC), message_sink(MH), data_sink(SF), worker(nullptr)
{
    assert(strcmp(Format(), IC.Format()) == 0);
    assert(strcmp(Format(), MH.Format()) == 0);
    assert(strcmp(Format(), SF.Format()) == 0);
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
