//
//  InputScanner.hpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 10.5.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#ifndef InputScanner_hpp
#define InputScanner_hpp


#include "RawData.hpp"
#include "InputChannel.hpp"
#include "StorageDataSink.hpp"
#include "MessageHandler.hpp"
#include "Output.hpp"
#include <vector>
#include <thread>
#include <tuple>


// Base class with derived classes overriding pure virtual members.
// Runs a thread that polls InputChannel so that it is as simple as possible.
// When finds something makes an item out of it.
class InputScanner {
protected:
    InputChannel& channel;
    MessageHandler& message_sink;
    StorageDataSink& data_sink;
    std::thread* worker;
    RawData buffer; // Given to channel and scanned.

public:
    enum Recipient {
        Discard = 0,
        DiscardRetroactively,
        Reset,
        Data,
        DataEnd,
        Message,
        MessageEnd
    };

protected:
    virtual
        std::tuple<Recipient, RawData::ConstIterator, RawData::ConstIterator>
        scan_input(Recipient Previous, RawData::ConstIterator RangeBegin,
            RawData::ConstIterator RangeEnd) = 0;

    friend void input_scanner(InputScanner* IS);

public:
    InputScanner(InputChannel& IC, MessageHandler& MH, StorageDataSink& SDS);
    virtual ~InputScanner();

    virtual const char* const Format() const = 0;

    bool Ended() const { return channel.Ended(); }
    void Scan(); // Starts scanning thread that runs until channel ends.
    void FinishScan();
};


extern void input_scanner(InputScanner* IS);


#endif /* InputScanner_hpp */
