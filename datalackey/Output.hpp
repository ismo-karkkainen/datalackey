//
//  Output.hpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 16.3.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#ifndef Output_hpp
#define Output_hpp


#include "ValueReference.hpp"
#include "Structure.hpp"
#include "Encoder.hpp"
#include "OutputChannel.hpp"
#include <vector>
#include <utility>

// We should use an array of dictionaries with each having just one key.
// That way any value output can be interleaved with others.
// And consequently, should the output be organized so that each Output object
// handles one key and they internally deal with taking turns at output?
// One dictionary, one value, written as soon as possible.
// The first to output can still write immediately. When the writer ends the
// output, others can dump. Or have one waiting buffer that receives object
// when non-writing output receives end?
// Removes a need to deal with nested output properly.

// Generalize to multiple output streams? A writer object that takes data in
// via operator<< but only needs to accept unsigned char array. Internally can
// deal with std::cout and std::cerr. A writer would be mapped to specific key
// or a default writer is used for all other keys.
// Hence program would have to map error output key to std::cerr writer or to
// nothing.

// We are meant to run a program. Everything in the output not recognized as
// something to store is passed to caller. Can we specify multiple means of
// communicating with program and deal with input from any channel regardless
// of content? That way we only need to concern with output that can not be
// recognized. 

// Just deal with output keys so that one can specify channel. Maybe channel
// name as extra parameter but if not recognized, use default channel. Or the
// name to channel mapping has been done previously. That way error key would
// be directed to stderr when available.


class Output;


// This receives the data that Item produces.
class ItemBuffer {
private:
    bool side_channel;
    bool ended;
    OutputChannel* channel;
    Output& master;
    std::vector<char> buffer;

    ItemBuffer(Output& Master, bool SideChannel);
    ~ItemBuffer();
    void SetChannel(OutputChannel* OC);

    bool Channel() const { return channel; }
    bool SideChannel() const { return side_channel; }
    size_t Size() const { return buffer.size(); }
    bool Ended() const { return ended; }

    friend class Output;

public:
    std::vector<char>* IntermediateBuffer();
    ItemBuffer& operator<<(const std::vector<char>& Data);
    void End(); // Indicates there will be no more data.
};

// User hands output to this and then deletes when not needed anymore.
class Item {
private:
    Encoder* encoder;
    std::vector<char> encoder_buffer;
    ItemBuffer& buffer;

    // Owns Encoder.
    Item(Encoder* E, ItemBuffer& B);

    friend class Output;
public:
    ~Item();
    Item& operator<<(Structure S);
    Item& operator<<(const ValueReference& VR);
};

class Output {
private:
    const Encoder& encoder;
    std::vector<ItemBuffer*> buffers;
    std::vector<std::pair<OutputChannel*,ItemBuffer*> > mains, sides;

    void AllocateChannels(bool SideChannel,
        std::vector<std::pair<OutputChannel*,ItemBuffer*> >& List);

public:
    Output(const Encoder& Format, OutputChannel& Main);
    ~Output();

    void AddChannel(OutputChannel& OC, bool SideChannel);

    Item* Writable(bool SideChannel = false);

    // For communication from ItemBuffer objects.
    void Ended(ItemBuffer& IB);
};

#endif /* Output_hpp */
