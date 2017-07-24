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
#include <vector>
#include <thread>
#include <tuple>


// Some rules for the input.
// A dictionary is treated as data container.
// An array is treated as item container? Or item in array is either a
// dictionary, treated as container, and anything else is treated as message.
// If top-level object is an array containing arrays or dictionaries, it is
// silently ignored.
// "Top"-level array is a message passed through from child to parent.
// Convert as needed. Special conversion item.
// "Top"-level dictionary is a data container. Avoid conversion as possible.
// Store as is for each data item. Special data item.
// "Top"-level array from parent is command with parameters and nothing else.
// Decode as appropriate and act. When data is set, there may be data block
// that should not be decoded but treated as data item.

// Only commands are something that needs to be understood. Otherwise just
// handle dictionary and keys and actual values are just data blocks.
// For messages a conversion may be needed and that is the same as for the
// values.

// Messages either pass through as they are or need format conversion.

// Values are stored as they are but when used the next time, may need a format
// conversion.

// Input needs to be scanned and redirected depending on what we have.

// Scanning needs to know whether to stop at next level or whether to continue
// until the item has been obtained completely.

// Item can be dictionary key or value or array containing message.

// Scanner object polls the InputChannel and scans what it gets and redirects
// to the outputs it has received. There has to be a pass-through for messages,
// a key/value receiver for the data. Scanner is format-specific.

// Input command looks like a message and could go to a "pass-through" that
// actually decodes the command. Analogous to output from child, we could pass
// data key/value pairs to storage so that they can be set directly. Command
// to set data could still exist.

// Hence decoder only for commands. Otherwise use converter if needed.
// Scanner scans to Structure and maybe other indicators to simplify the
// possible decoding and other stuff? Still values may end up coming piece-meal
// for large values. When scanning needs to know the structure to find the
// object boundaries.

// When decoding, the decoder knows what has passed so it will know what to do
// to the next item.


// Would it be easier to assign encoding and decoding on per channel? Flexible
// but ever used? Complicates setting up communication. Retain possibility of
// supporting it in the future.


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
        Data,
        DataEnd,
        Message,
        MessageEnd
    };

protected:
    virtual std::tuple<Recipient, RawData::Iterator, RawData::Iterator>
        scan_input(Recipient Previous,
            RawData::Iterator RangeBegin, RawData::Iterator RangeEnd) = 0;

    friend void input_scanner(InputScanner* IS);

public:
    InputScanner(InputChannel& IC, MessageHandler& MH, StorageDataSink& SDS);
    virtual ~InputScanner();

    virtual const char* const Format() const = 0;

    bool Ended() const { return channel.Ended(); }
    void Scan(); // Starts scanning thread that runs until channel ends.
};


extern void input_scanner(InputScanner* IS);


#endif /* InputScanner_hpp */
