//
//  Output.hpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 16.3.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//
// Licensed under Universal Permissive License. See License.txt.

#ifndef Output_hpp
#define Output_hpp


#include "ValueReference.hpp"
#include "Structure.hpp"
#include "Encoder.hpp"
#include "OutputChannel.hpp"
#include "RawData.hpp"
#include "OutputItem.hpp"
#include "ProcessInput.hpp"
#include "SimpleValue.hpp"

class Output;

#include "InputScanner.hpp"
#include "Message.hpp"
#include <vector>
#include <mutex>
#include <queue>
#include <thread>
#include <condition_variable>
#include <set>
#include <memory>


// This receives the data that OutputItem produces.
class OutputItemBuffer {
private:
    bool ended;
    Output* master;
    RawData buffer;
    std::mutex master_mutex;

    OutputItemBuffer(Output& Master);
    OutputItemBuffer(const OutputItemBuffer&) = delete;
    OutputItemBuffer& operator=(const OutputItemBuffer&) = delete;

    void Orphan();

    friend class Output;

public:
    ~OutputItemBuffer();
    RawData* Buffer() { return &buffer; }

    OutputItemBuffer& operator<<(const RawData& Data);
    void Write(RawData::ConstIterator& Start, RawData::ConstIterator& End);

    void End(std::shared_ptr<OutputItemBuffer>& This); // No more data.
};


// User hands output to this and then deletes when not needed anymore.
class OutputItemWriter : public OutputItem {
private:
    Encoder* encoder;
    RawData encoder_buffer;
    std::shared_ptr<OutputItemBuffer> buffer;

    // Owns Encoder.
    OutputItemWriter(Encoder* E, std::shared_ptr<OutputItemBuffer>& B);
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
    class Batch {
    public:
        std::vector<std::shared_ptr<ProcessInput>> items;
    };
    // Lock input_sets_mutex first, then this if you need both.
    mutable std::mutex mutex;
    const Encoder& encoder;
    std::set<std::shared_ptr<OutputItemBuffer>> buffers;

    OutputChannel& channel;
    std::thread* channel_feeder;
    std::condition_variable output_added;
    std::queue<Batch> inputs;
    std::queue<std::shared_ptr<OutputItemBuffer>> ended_buffers;
    bool eof;
    mutable std::mutex input_sets_mutex;

    bool failed;

    void orphan_buffers(); // Locks nothing. Lock as needed.
    void clear_buffers();
    void feed_data(const RawData* Data) noexcept(false);
    void feed_key_value(std::shared_ptr<ProcessInput> KeyValue,
        Encoder* E, RawData& Deco) noexcept(false);
    void feed_object(Batch& B) noexcept(false);
    void feed_messages(std::unique_lock<std::mutex>& Lock) noexcept(false);
    void feeder();
    void end(bool FromOutside);

public:
    Output(const Encoder& E, OutputChannel& Main,
        bool DataNotify = true, bool ProcessNotify = true);
    ~Output();

    void NoGlobalMessages();

    const char *const Format() const { return encoder.Format(); }

    OutputItem* Writable(bool Discarder = false);
    void Feed(std::vector<std::shared_ptr<ProcessInput>>& Inputs);
    void End() { end(true); }
    bool Closed() const { return eof; }

    // Failure on reading input data or write failure.
    bool Failed() const { return failed; }
    // True when there is no expected output anywhere at the moment.
    bool Finished() const;

    // For communication from OutputItemBuffer objects.
    void Ended(std::shared_ptr<OutputItemBuffer>& IB);
};


#endif /* Output_hpp */
