//
//  Output.cpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 16.3.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#include "Output.hpp"
#include "OutputCollection.hpp"
#include "Value_t.hpp"
#include "Structure.hpp"
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


void Output::ReadError::Report(Output& Out) const {
    Send(Out, &Message::id, Message::item);
    Send(Out, nullptr, Message::item);
}

void Output::ReadError::Send(
    Output& Out, const SimpleValue* Id, const char* const Label) const
{
    if (Id == nullptr)
        message(Out, "read", "error", Label);
    else
        message(Out, Id, "read", "error", Label);
}


Output* Output::first = nullptr;


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

void Output::feeder() {
    std::shared_ptr<ProcessInput> input;
    std::shared_ptr<const RawData> rd;
    std::shared_ptr<OutputItemBuffer> outbuf = nullptr;
    const RawData* data = nullptr;
    RawData deco;
    size_t written = 0;
    size_t slice_size = 32768; // Get from OutputChannel.
    bool in_input = false;
    std::unique_lock<std::mutex> lock(input_sets_mutex, std::defer_lock);
    while (true) {
        if (data != nullptr) { // Keep writing data we have.
            size_t left = data->Size() - written;
            if (slice_size < left)
                left = slice_size;
            RawData::ConstIterator start = data->CBegin() + written;
            RawData::ConstIterator end = data->CBegin() + written + left;
            written += channel.Write(start, end);
            if (channel.Failed()) {
                failed = true;
                this->end(false);
                written = data->Size();
                break;
            }
            if (written == data->Size()) {
                if (outbuf != nullptr && data == outbuf->Buffer()) {
                    outbuf = nullptr;
                } else if (data == rd.get()) {
                    rd = nullptr;
                } else
                    deco.Clear();
                data = nullptr;
                written = 0;
            }
            // Write bookkeeping happens here.
            continue;
        }
        if (input) { // Need new block to write.
            rd = input->Data()->Read(1048576); // Should depend on slice_size.
            if (rd) {
                data = rd.get();
            } else {
                if (input->Data()->Error()) {
                    if (this != Output::first) {
                        DataNotifiedOutputs.Notify(controller_output,
                            [this, &input](Output* Out) { read_error.Send(
                                *Out, controller_output_identifier,
                                input->Label()->String().c_str()); },
                            [this, &input](Output* Out) {
                                if (Out == Output::first) read_error.Send(
                                    *Out, nullptr,
                                    input->Label()->String().c_str()); });
                        // No idea what the command is. Run, feed, get?
                        // Technically bad enough to warrant special handling
                        // in controller. Identifier is that of run command.
                        // Must add format once supported.
                    }
                    failed = true;
                    break;
                }
                input = nullptr; // Finished this one.
            }
            continue;
        }
        if (in_input) { // Get next input from queue.
            lock.lock();
            input = inputs.front();
            inputs.pop();
            lock.unlock();
            if (input) {
                // Buffer has to have separator and then name.
                Encoder* e = encoder.Clone();
                e->Encode(deco, Dictionary);
                e->Encode(deco, ValueRef<std::string>(input->Name()->String()));
                e->Encode(deco, RawItem);
                deco.Clear();
                // Now separator, name, separator for value.
                e->Encode(deco, ValueRef<std::string>(input->Name()->String()));
                e->Encode(deco, RawItem);
                data = deco.Size() ? &deco : nullptr;
                delete e;
                continue;
            }
            // We got nullptr, finish the dictionary.
            in_input = false;
            Encoder* e = encoder.Clone();
            e->Encode(deco, Dictionary);
            e->Encode(deco, ValueRef<std::string>(std::string("")));
            e->Encode(deco, RawItem);
            deco.Clear();
            e->Encode(deco, Structure::End);
            data = deco.Size() ? &deco : nullptr;
            delete e;
            continue;
        }
        lock.lock();
        if (!ended_buffers.empty()) { // Has a queued message to send.
            outbuf = ended_buffers.front();
            ended_buffers.pop();
            lock.unlock();
            data = outbuf->Buffer();
            continue;
        }
        if (!inputs.empty()) { // Has next batch of inputs to process.
            input = inputs.front();
            inputs.pop();
            lock.unlock();
            if (input == nullptr) { // Empty object.
                Encoder* e = encoder.Clone();
                e->Encode(deco, Dictionary);
                e->Encode(deco, Structure::End);
                data = &deco;
                delete e;
                continue;
            }
            if (input->Name() == nullptr) {
                input = nullptr; // Just a marker, nothing to output.
                break;
            }
            in_input = true;
            Encoder* e = encoder.Clone();
            e->Encode(deco, Dictionary);
            e->Encode(deco, ValueRef<std::string>(input->Name()->String()));
            e->Encode(deco, RawItem);
            data = &deco;
            delete e;
            continue;
        }
        // We had nothing to write. Wait for something (unfinished) to arrive.
        channel.Flush();
        output_added.wait(lock);
        lock.unlock();
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
        std::shared_ptr<ProcessInput> marker(new ProcessInput());
        std::unique_lock<std::mutex> lock(input_sets_mutex);
        inputs.push(marker);
        orphan_buffers();
        lock.unlock();
        output_added.notify_one();
    }
}

Output::Output(const Encoder& E, OutputChannel& Main, bool DataNotify,
    bool ProcessNotify, Output* ControllerOutput, SimpleValue* Identifier)
    : controller_output(ControllerOutput),
    controller_output_identifier(Identifier), encoder(E), channel(Main),
    channel_feeder(nullptr), eof(false), failed(false)
{
    if (encoder.Format() == nullptr)
        eof = true;
    else {
        channel_feeder = new std::thread(&Output::feeder, this);
        if (Output::first == nullptr)
            Output::first = this;
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
    std::unique_lock<std::mutex> lock(input_sets_mutex);
    for (auto& input : Inputs)
        inputs.push(input);
    // This indicates that the input group ended.
    inputs.push(std::shared_ptr<ProcessInput>());
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
