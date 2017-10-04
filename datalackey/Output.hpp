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
#include "RawData.hpp"

class Output;

#include "InputScanner.hpp"
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


// This receives the data that OutputItem produces.
class OutputItemBuffer {
private:
    bool side_channel;
    bool ended;
    OutputChannel* channel;
    Output& master;
    RawData buffer;

    OutputItemBuffer(Output& Master, bool SideChannel);
    ~OutputItemBuffer();
    void SetChannel(OutputChannel* OC);

    OutputChannel* Channel() const { return channel; }
    bool SideChannel() const { return side_channel; }
    size_t Size() const { return buffer.Size(); }
    bool Ended() const { return ended; }

    friend class Output;

public:
    RawData* IntermediateBuffer();
    // Normal output.
    OutputItemBuffer& operator<<(const RawData& Data);
    // Pass-through from input.
    void Write(RawData::ConstIterator& Start, RawData::ConstIterator& End);
    void End(); // Indicates there will be no more data.
};

// User hands output to this and then deletes when not needed anymore.
class OutputItem {
private:
    Encoder* encoder;
    RawData encoder_buffer;
    OutputItemBuffer& buffer;

    // Owns Encoder.
    OutputItem(Encoder* E, OutputItemBuffer& B);

    friend class Output;
public:
    ~OutputItem();
    OutputItem& operator<<(Structure S);
    OutputItem& operator<<(const ValueReference& VR);
    void Write(RawData::ConstIterator Start, RawData::ConstIterator End);
};

class Output {
private:
    const Encoder& encoder;
    std::vector<OutputItemBuffer*> buffers;
    std::vector<std::pair<OutputChannel*,OutputItemBuffer*> > mains, sides;

    void AllocateChannels(bool SideChannel,
        std::vector<std::pair<OutputChannel*,OutputItemBuffer*> >& List);

public:
    Output(const Encoder& E, OutputChannel& Main);
    ~Output();

    const char *const Format() const { return encoder.Format(); }

    void AddChannel(OutputChannel& OC, bool SideChannel);

    OutputItem* Writable(bool SideChannel = false);

    // For communication from OutputItemBuffer objects.
    void Ended(OutputItemBuffer& IB);
};

#endif /* Output_hpp */
