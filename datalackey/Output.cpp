//
//  Output.cpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 16.3.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#include "Output.hpp"


OutputItemBuffer::OutputItemBuffer(Output& Master, bool SideChannel)
    : side_channel(SideChannel), ended(false), channel(nullptr),
    master(Master)
{ }

void OutputItemBuffer::SetChannel(OutputChannel* OC) {
    channel = OC;
    if (buffer.size() != 0) {
        *channel << buffer;
        buffer = std::vector<char>();
    }
}

OutputItemBuffer::~OutputItemBuffer() {
}

std::vector<char>* OutputItemBuffer::IntermediateBuffer() {
    return ((channel != nullptr) || ended) ? nullptr : &buffer;
}

OutputItemBuffer& OutputItemBuffer::operator<<(const std::vector<char>& Data) {
    if (channel != nullptr)
        *channel << Data;
    else
        buffer.insert(buffer.end(), Data.cbegin(), Data.cend());
    return *this;
}

void OutputItemBuffer::Write(
    InputScanner::Iterator& Start, InputScanner::Iterator& End)
{
    if (channel != nullptr)
        channel->Write(Start, End);
    else
        buffer.insert(buffer.end(), Start, End);
}

void OutputItemBuffer::End() {
    if (channel != nullptr && !buffer.empty()) {
        *channel << buffer;
        buffer.resize(0);
    }
    if (!ended)
        master.Ended(*this);
    ended = true;
}


OutputItem::OutputItem(Encoder* E, OutputItemBuffer& B)
    : encoder(E), buffer(B)
{ }

OutputItem::~OutputItem() {
    buffer.End();
    delete encoder;
}

OutputItem& OutputItem::operator<<(Structure S) {
    std::vector<char>* buf = buffer.IntermediateBuffer();
    if (encoder->EncodeOutputsDirectly() && buf != nullptr) {
        // Writing directly to output buffer.
        encoder->Encode(*buf, S);
        return *this;
    }
    if (encoder->Encode(encoder_buffer, S)) {
        buffer << encoder_buffer;
        encoder_buffer.resize(0);
    }
    return *this;
}

OutputItem& OutputItem::operator<<(const ValueReference& VR) {
    std::vector<char>* buf = buffer.IntermediateBuffer();
    if (encoder->EncodeOutputsDirectly() && buf != nullptr) {
        // Writing directly to output buffer.
        encoder->Encode(*buf, VR);
        return *this;
    }
    if (encoder->Encode(encoder_buffer, VR)) {
        buffer << encoder_buffer;
        encoder_buffer.resize(0);
    }
    return *this;
}

void OutputItem::Write(
    InputScanner::Iterator& Start, InputScanner::Iterator& End)
{
    buffer.Write(Start, End);
}


void Output::AllocateChannels(bool SideChannel,
    std::vector<std::pair<OutputChannel*,OutputItemBuffer*> >& List)
{
    if (buffers.empty()) // Nothing to use the channel.
        return;
    // Find an available channel.
    std::pair<OutputChannel*,OutputItemBuffer*>* available = nullptr;
    for (size_t k = 0; k < List.size(); ++k)
        if (List[k].second == nullptr) {
            available = &List[k];
            break;
        }
    if (available == nullptr)
        return;
    // Get ended buffers out of the way first.
    for (size_t k = buffers.size(); k; --k) {
        OutputItemBuffer* current = buffers[k - 1];
        if (!current->Ended() || current->SideChannel() != SideChannel)
            continue;
        if (current->Size())
            current->SetChannel(available->first); // Writes out all.
        delete current;
        buffers[k - 1] = buffers.back();
        buffers.pop_back();
    }
    // Find buffer with most data.
    OutputItemBuffer* largest = nullptr;
    for (auto b : buffers) {
        if (b->Channel() != nullptr)
            continue;
        if (largest == nullptr || largest->Size() < b->Size())
            largest = b;
    }
    if (largest == nullptr)
        return;
    available->second = largest;
    largest->SetChannel(available->first);
}

Output::Output(const Encoder& E, OutputChannel& Main)
    : encoder(E)
{
    mains.push_back(std::make_pair<OutputChannel*,OutputItemBuffer*>(&Main, nullptr));
}

Output::~Output() {
    // Lock.
    // An opportunity to check for OutputItems not yet done with.
    // Though we could be exiting on exception or something.
    // And where exactly are we going to report? This is the output.
    // Write to file in current directory maybe?
    for (auto& buffer : buffers)
        delete buffer;
    // OutputChannels allocated and owned outside.
}

void Output::AddChannel(OutputChannel& OC, bool SideChannel) {
    // Lock.
    if (SideChannel) {
        sides.push_back(std::make_pair<OutputChannel*,OutputItemBuffer*>(&OC, nullptr));
        AllocateChannels(true, sides);
    } else {
        mains.push_back(std::make_pair<OutputChannel*,OutputItemBuffer*>(&OC, nullptr));
        AllocateChannels(false, mains);
    }
}

OutputItem* Output::Writable(bool SideChannel) {
    // Lock.
    OutputItemBuffer* buffer = new OutputItemBuffer(*this, SideChannel);
    buffers.push_back(buffer);
    AllocateChannels(SideChannel,
        (SideChannel && !sides.empty()) ? sides : mains);
    return new OutputItem(encoder.CreateSame(), *buffer);
}

void Output::Ended(OutputItemBuffer& IB) {
    // Lock.
    // Should find and free if ended and zero-size even if no channel?
    if (IB.Channel() == nullptr)
        return;
    for (size_t k = 0; k < mains.size(); ++k)
        if (mains[k].second == &IB) {
            mains[k].second = nullptr;
            AllocateChannels(false, mains);
            return;
        }
    for (size_t k = 0; k < sides.size(); ++k)
        if (sides[k].second == &IB) {
            sides[k].second = nullptr;
            AllocateChannels(true, sides);
            return;
        }
}
