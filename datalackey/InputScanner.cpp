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


InputScanner::InputScanner(InputChannel& IC, MessageHandler& MH,
    StorageDataSink& SDS)
    : channel(IC), message_sink(MH), data_sink(SDS), previous(Discard)
{ }

InputScanner::~InputScanner() {
}

bool InputScanner::Scan() {
    bool had_input = false;
    while (!channel.Ended() && channel.Read(buffer)) {
        had_input = true;
        // Use all data piece by piece and keep track of used part.
        RawData::ConstIterator begin = buffer.CBegin();
        RawData::ConstIterator end = begin;
        do {
            bool bad = false;
            Recipient recipient;
            std::tie(recipient, begin, end) =
                scan_input(previous, begin, buffer.CEnd());
            switch (recipient) {
            case Discard:
                break; // Throw blank data away.
            case Reset:
                // Same action as with DiscardRetroactively.
            case DiscardRetroactively:
                // If passing data in, tell to discard it.
                if (previous == Data)
                    data_sink.Discard(begin, end);
                else if (previous == Message)
                    message_sink.Discard(begin, end);
                break;
            case Data:
                bad = !data_sink.Input(begin, end);
                break;
            case DataEnd:
                bad = !data_sink.Input(begin, end);
                bad = !data_sink.End() || bad;
                recipient = Discard;
                break;
            case Message:
                bad = !message_sink.Input(begin, end);
                break;
            case MessageEnd:
                bad = !message_sink.Input(begin, end);
                bad = !message_sink.End() || bad;
                recipient = Discard;
                break;
            }
            if (bad)
                recipient = DiscardRetroactively;
            begin = end;
            previous = recipient;
        } while (end != buffer.CEnd());
        buffer.Clear();
    }
    return had_input;
}
