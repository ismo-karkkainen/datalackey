//
//  Output.cpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 16.3.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#include "Output.hpp"


OutputItemBuffer::OutputItemBuffer(Output& Master)
    : ended(false), channel(nullptr), master(Master)
{ }

void OutputItemBuffer::SetChannel(OutputChannel* OC) {
    channel = OC;
    if (buffer.Size() != 0) {
        *channel << buffer;
        buffer.Clear(true);
    }
}

OutputItemBuffer::~OutputItemBuffer() {
}

RawData* OutputItemBuffer::IntermediateBuffer() {
    return ((channel != nullptr) || ended) ? nullptr : &buffer;
}

OutputItemBuffer& OutputItemBuffer::operator<<(const RawData& Data) {
    if (channel != nullptr)
        *channel << Data;
    else
        buffer.Append(Data.CBegin(), Data.CEnd());
    return *this;
}

void OutputItemBuffer::Write(
    RawData::ConstIterator& Start, RawData::ConstIterator& End)
{
    if (channel != nullptr)
        channel->Write(Start, End);
    else
        buffer.Append(Start, End);
}

void OutputItemBuffer::End() {
    if (channel != nullptr && !buffer.Empty()) {
        *channel << buffer;
        buffer.Clear(true);
    }
    if (!ended) {
        ended = true;
        master.Ended(*this);
    }
}


OutputItemWriter::OutputItemWriter(Encoder* E, OutputItemBuffer& B)
    : encoder(E), buffer(B)
{ }

OutputItemWriter::~OutputItemWriter() {
    buffer.End();
    delete encoder;
}

OutputItem& OutputItemWriter::operator<<(Structure S) {
    RawData* buf = buffer.IntermediateBuffer();
    if (encoder->EncodeOutputsDirectly() && buf != nullptr) {
        // Writing directly to output buffer.
        encoder->Encode(*buf, S);
        return *this;
    }
    if (encoder->Encode(encoder_buffer, S)) {
        buffer << encoder_buffer;
        encoder_buffer.Clear();
    }
    return *this;
}

OutputItem& OutputItemWriter::operator<<(const ValueReference& VR) {
    RawData* buf = buffer.IntermediateBuffer();
    if (encoder->EncodeOutputsDirectly() && buf != nullptr) {
        // Writing directly to output buffer.
        encoder->Encode(*buf, VR);
        return *this;
    }
    if (encoder->Encode(encoder_buffer, VR)) {
        buffer << encoder_buffer;
        encoder_buffer.Clear();
    }
    return *this;
}

void OutputItemWriter::Write(
    RawData::ConstIterator Start, RawData::ConstIterator End)
{
    buffer.Write(Start, End);
}


void Output::AllocateChannel(OutputItemBuffer* PreviousWriter) {
    if (buffers.empty()) // Nothing to use the channel.
        return;
    if (writer != nullptr)
        return;
    // Get ended buffers out of the way first.
    for (size_t k = buffers.size(); k; --k) {
        OutputItemBuffer* current = buffers[k - 1];
        if (!current->Ended())
            continue;
        if (current != PreviousWriter && current->Size())
            current->SetChannel(&main); // Writes out all it has stored.
        delete current;
        buffers[k - 1] = buffers.back();
        buffers.pop_back();
    }
    // Find buffer with most data. Or least? One with most can write it onwards.
    for (auto b : buffers) {
        if (writer == nullptr || writer->Size() < b->Size())
            writer = b;
    }
    if (writer != nullptr)
        writer->SetChannel(&main);
}

Output::Output(const Encoder& E, OutputChannel& Main)
    : encoder(E), main(Main), writer(nullptr)
{ }

Output::~Output() {
    for (auto& buffer : buffers)
        delete buffer;
    // OutputChannel allocated and owned outside.
}

OutputItem* Output::Writable(bool Discarder) {
    if (Discarder)
        return new OutputItemDiscarder();
    OutputItemBuffer* buffer = new OutputItemBuffer(*this);
    std::lock_guard<std::mutex> lock(mutex);
    buffers.push_back(buffer);
    AllocateChannel();
    return new OutputItemWriter(encoder.Clone(), *buffer);
}

void Output::Ended(OutputItemBuffer& IB) {
    if (&IB != writer)
        return;
    main.Flush();
    std::lock_guard<std::mutex> lock(mutex);
    writer = nullptr;
    AllocateChannel(&IB);
}
