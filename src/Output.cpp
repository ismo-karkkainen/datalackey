//
//  Output.cpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 16.3.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//
// Licensed under Universal Permissive License. See License.txt.

#include "Output.hpp"
#include "OutputCollection.hpp"
#include "Value_t.hpp"
#include "Structure.hpp"
#include "Messages.hpp"
#include <cassert>


OutputItemBuffer::OutputItemBuffer(Output& Master)
    : ended(false), master(&Master)
{ }

void OutputItemBuffer::Orphan() {
    std::lock_guard<std::mutex> lock(master_mutex);
    master = nullptr;
    ended = true; // Stops further accumulation of output.
}

OutputItemBuffer::~OutputItemBuffer() { }

OutputItemBuffer& OutputItemBuffer::operator<<(const RawData& Data) {
    RawData::ConstIterator start = Data.CBegin();
    RawData::ConstIterator end = Data.CEnd();
    Write(start, end);
    return *this;
}

void OutputItemBuffer::Write(
    RawData::ConstIterator& Start, RawData::ConstIterator& End)
{
    if (!ended)
        buffer.Append(Start, End);
}

void OutputItemBuffer::End(std::shared_ptr<OutputItemBuffer>& This) {
    if (!ended) {
        ended = true;
        std::lock_guard<std::mutex> lock(master_mutex);
        if (master)
            master->Ended(This);
    }
}


OutputItemWriter::OutputItemWriter(
    Encoder* E, std::shared_ptr<OutputItemBuffer>& B)
    : encoder(E), buffer(B)
{ }

OutputItemWriter::~OutputItemWriter() {
    buffer->End(buffer);
    delete encoder;
}

OutputItem& OutputItemWriter::operator<<(Structure S) {
    if (encoder->EncodeOutputsDirectly()) {
        encoder->Encode(*(buffer->Buffer()), S);
    } else if (encoder->Encode(encoder_buffer, S)) {
        *buffer << encoder_buffer;
        encoder_buffer.Clear();
    }
    return *this;
}

OutputItem& OutputItemWriter::operator<<(const ValueReference& VR) {
    if (encoder->EncodeOutputsDirectly()) {
        encoder->Encode(*(buffer->Buffer()), VR);
    } else if (encoder->Encode(encoder_buffer, VR)) {
        *buffer << encoder_buffer;
        encoder_buffer.Clear();
    }
    return *this;
}

void OutputItemWriter::Write(
    RawData::ConstIterator Start, RawData::ConstIterator End)
{
    buffer->Write(Start, End);
}


void Output::orphan_buffers() {
    for (auto& buf : buffers)
        buf->Orphan();
    buffers.clear();
}

void Output::clear_buffers() {
    std::lock_guard<std::mutex> lock(input_sets_mutex);
    std::lock_guard<std::mutex> lock2(mutex);
    orphan_buffers();
    while (!ended_buffers.empty())
        ended_buffers.pop();
    while (!inputs.empty())
        inputs.pop();
}

void Output::feed_data(const RawData* Data) noexcept(false) {
    size_t written = 0;
    const size_t slice_size = 65536; // Get from OutputChannel.
    while (written < Data->Size()) {
        size_t left = Data->Size() - written;
        if (slice_size < left)
            left = slice_size;
        RawData::ConstIterator start = Data->CBegin() + written;
        RawData::ConstIterator end = Data->CBegin() + written + left;
        written += channel.Write(start, end);
        if (channel.Failed())
            throw false;
    }
}

void Output::feed_key_value(std::shared_ptr<ProcessInput> KeyValue,
    Encoder* E, RawData& Deco) noexcept(false)
{
    E->Encode(Deco, ValueRef<std::string>(KeyValue->Name()->String()));
    E->Encode(Deco, RawItem);
    feed_data(&Deco);
    Deco.Clear();
    while (true) {
        std::shared_ptr<const RawData> rd = KeyValue->Data()->Read(1048576);
        if (rd) {
            feed_data(rd.get());
            continue;
        }
        if (KeyValue->Data()->Error()) {
            DataNotifiedOutputs.Notify([&KeyValue](Output* Out) {
                ntf_data_error.Send(*Out,
                    KeyValue->Label()->String().c_str(), 0);
            });
            throw true;
        }
        return; // Read all data and are done with the value.
    }
}

void Output::feed_object(Batch& B) noexcept(false) {
    RawData deco;
    std::unique_ptr<Encoder> e(encoder.Clone());
    e->Encode(deco, Dictionary);
    for (auto& kv : B.items)
        feed_key_value(kv, e.get(), deco);
    e->Encode(deco, Structure::End);
    feed_data(&deco);
}

void Output::feed_messages(std::unique_lock<std::mutex>& Lock) noexcept(false) {
    Lock.lock();
    while (!ended_buffers.empty()) { // Has queued messages to send.
        std::shared_ptr<OutputItemBuffer> outbuf = ended_buffers.front();
        ended_buffers.pop();
        Lock.unlock();
        feed_data(outbuf->Buffer());
        Lock.lock();
    }
    Lock.unlock();
}

void Output::feeder() {
    std::unique_lock<std::mutex> lock(input_sets_mutex, std::defer_lock);
    while (true) {
        try {
            feed_messages(lock); // Give priority to notifications.
            lock.lock();
            if (inputs.empty()) {
                if (Closed())
                    break;
                // We had nothing to write. Wait for something to arrive.
                channel.Flush();
                output_added.wait(lock);
                lock.unlock();
                continue;
            }
            Batch b = inputs.front();
            inputs.pop();
            lock.unlock();
            feed_object(b);
        }
        catch (bool failure) {
            this->end(false);
            break;
        }
    }
    channel.Close();
    // On failure we may have references to data in input queue.
    if (lock)
        lock.unlock();
    clear_buffers();
}

void Output::end(bool FromOutside) {
    if (!eof) {
        eof = true;
        if (!FromOutside)
            return;
        std::unique_lock<std::mutex> lock(input_sets_mutex);
        orphan_buffers();
        lock.unlock();
        output_added.notify_one();
    }
}

Output::Output(const Encoder& E, OutputChannel& Main, bool DataNotify,
    bool ProcessNotify)
    : encoder(E), channel(Main), channel_feeder(nullptr),
    eof(false), failed(false)
{
    if (encoder.Format() == nullptr)
        eof = true;
    else {
        channel_feeder = new std::thread(&Output::feeder, this);
        if (DataNotify)
            DataNotifiedOutputs.Add(this);
        if (ProcessNotify)
            ProcessNotifiedOutputs.Add(this);
    }
}

Output::~Output() {
    NoGlobalMessages();
    if (channel_feeder != nullptr) {
        End();
        channel_feeder->join();
        delete channel_feeder;
    }
    channel.Close();
    clear_buffers();
    // OutputChannel allocated and owned outside.
}

void Output::NoGlobalMessages() {
    DataNotifiedOutputs.Remove(this);
    ProcessNotifiedOutputs.Remove(this);
}

OutputItem* Output::Writable(bool Discarder) {
    if (Discarder || channel.Failed() || eof)
        return new OutputItemDiscarder();
    std::shared_ptr<OutputItemBuffer> buffer(new OutputItemBuffer(*this));
    std::unique_lock<std::mutex> lock(mutex);
    buffers.insert(buffer);
    lock.unlock();
    return new OutputItemWriter(encoder.Clone(), buffer);
}

void Output::Feed(std::vector<std::shared_ptr<ProcessInput>>& Inputs) {
    if (eof)
        return;
    Batch batch;
    batch.items = Inputs;
    std::unique_lock<std::mutex> lock(input_sets_mutex);
    inputs.push(batch);
    lock.unlock();
    output_added.notify_one();
}

bool Output::Finished() const {
    std::lock_guard<std::mutex> lock2(input_sets_mutex);
    std::lock_guard<std::mutex> lock(mutex);
    return buffers.empty() && ended_buffers.empty();
}

void Output::Ended(std::shared_ptr<OutputItemBuffer>& IB) {
    std::unique_lock<std::mutex> lock2(input_sets_mutex);
    std::unique_lock<std::mutex> lock(mutex);
    buffers.erase(IB);
    if (channel.Failed() || failed || eof || channel.Closed())
        return;
    ended_buffers.push(IB);
    lock.unlock();
    lock2.unlock();
    output_added.notify_one();
}
