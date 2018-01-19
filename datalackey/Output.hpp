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
#include "OutputItem.hpp"

class Output;

#include "InputScanner.hpp"
#include <vector>
#include <utility>
#include <mutex>
#include <set>


// This receives the data that OutputItem produces.
class OutputItemBuffer {
private:
    bool ended;
    OutputChannel* channel;
    Output& master;
    RawData buffer;

    OutputItemBuffer(Output& Master);
    ~OutputItemBuffer();
    void SetChannel(OutputChannel* OC);

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
class OutputItemWriter : public OutputItem {
private:
    Encoder* encoder;
    RawData encoder_buffer;
    OutputItemBuffer& buffer;

    // Owns Encoder.
    OutputItemWriter(Encoder* E, OutputItemBuffer& B);

    friend class Output;

public:
    ~OutputItemWriter();
    OutputItem& operator<<(Structure S);
    OutputItem& operator<<(const ValueReference& VR);
    void Write(RawData::ConstIterator Start, RawData::ConstIterator End);
};

class Output {
private:
    std::mutex mutex;
    const Encoder& encoder;
    std::vector<OutputItemBuffer*> buffers;
    OutputChannel& main;
    OutputItemBuffer* writer;

    void AllocateChannel(OutputItemBuffer* PreviousWriter = nullptr);

public:
    Output(const Encoder& E, OutputChannel& Main, bool GlobalMessages = true);
    ~Output();

    void NoGlobalMessages();

    const char *const Format() const { return encoder.Format(); }

    OutputItem* Writable(bool Discarder = false);

    // True when there is no expected output anywhere at the moment.
    bool Finished() const { return buffers.empty(); }

    // For communication from OutputItemBuffer objects.
    void Ended(OutputItemBuffer& IB);
};


// For storing pointers to all output objects.
class OutputCollection {
private:
    std::set<Output*> collection;
    std::mutex mutex;

    friend class Output;

    void Add(Output* O);
    void Remove(Output* O);

public:
    std::mutex& Mutex() { return mutex; } // Lock first.
    std::vector<Output*> Outputs() const { // Presumes previous is locked.
        return std::vector<Output*>(collection.begin(), collection.end());
    }
};

extern OutputCollection GloballyMessageableOutputs;


#endif /* Output_hpp */
