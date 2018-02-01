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
#include "ProcessInput.hpp"

class Output;

#include "InputScanner.hpp"
#include <vector>
#include <mutex>
#include <queue>
#include <thread>
#include <condition_variable>
#include <set>


// This receives the data that OutputItem produces.
class OutputItemBuffer {
private:
    bool ended;
    Output& master;
    RawData buffer;

    OutputItemBuffer(Output& Master);
    OutputItemBuffer(const OutputItemBuffer&) = delete;
    OutputItemBuffer& operator=(const OutputItemBuffer&) = delete;
    ~OutputItemBuffer();

    friend class Output;

public:
    RawData* Buffer() { return &buffer; }

    OutputItemBuffer& operator<<(const RawData& Data);
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
    OutputItemWriter(const OutputItemWriter&) = delete;
    OutputItemWriter& operator=(const OutputItemWriter&) = delete;

    friend class Output;

public:
    ~OutputItemWriter();
    OutputItem& operator<<(Structure S);
    OutputItem& operator<<(const ValueReference& VR);
    void Write(RawData::ConstIterator Start, RawData::ConstIterator End);
};


class Output {
private:
    static Output* first;

    // Not necessarily valid, use GloballyMessageableOutputs.
    Output* controller_output;

    mutable std::mutex mutex;
    const Encoder& encoder;
    std::set<OutputItemBuffer*> buffers;

    OutputChannel& channel;
    bool terminate;
    std::thread* channel_feeder;
    std::condition_variable output_added;
    std::queue<std::shared_ptr<ProcessInput>> inputs;
    std::queue<OutputItemBuffer*> ended_buffers;
    bool eof;
    mutable std::mutex input_sets_mutex;

    bool failed;

    void clear_buffers();
    void feeder();

public:
    Output(const Encoder& E, OutputChannel& Main, bool GlobalMessages = true,
        Output* ControllerOutput = nullptr);
    ~Output();

    void NoGlobalMessages();
    Output* ControllerOutput() { return controller_output; }

    const char *const Format() const { return encoder.Format(); }

    OutputItem* Writable(bool Discarder = false);
    void Feed(std::vector<std::shared_ptr<ProcessInput>>& Inputs);
    void End();
    bool Closed() const { return eof; }

    // Failure on reading input data or write failure.
    bool Failed() const { return failed; }
    // True when there is no expected output anywhere at the moment.
    bool Finished() const;

    // For communication from OutputItemBuffer objects.
    void Ended(OutputItemBuffer& IB);
};


// Stores pointers to output objects that want to or can receive notifications.
class OutputCollection {
private:
    std::set<Output*> collection;
    std::mutex mutex;

    friend class Output;

    void Add(Output* O);
    void Remove(Output* O);

public:
    std::mutex& Mutex() { return mutex; } // Lock first.
    std::vector<Output*> Outputs() const { // Previous locked for use duration.
        return std::vector<Output*>(collection.begin(), collection.end());
    }
};

extern OutputCollection GloballyMessageableOutputs;


#endif /* Output_hpp */
