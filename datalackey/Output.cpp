//
//  Output.cpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 16.3.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#include "Output.hpp"


ItemBuffer::ItemBuffer(Output& Master, bool SideChannel)
    : side_channel(SideChannel), ended(false), channel(nullptr),
    master(Master)
{ }

void ItemBuffer::SetChannel(OutputChannel* OC) {
    channel = OC;
    if (buffer.size() != 0) {
        *channel << buffer;
        buffer = std::vector<char>();
    }
}

ItemBuffer::~ItemBuffer() {
}

std::vector<char>* ItemBuffer::IntermediateBuffer() {
    return ((channel != nullptr) || ended) ? nullptr : &buffer;
}

ItemBuffer& ItemBuffer::operator<<(const std::vector<char>& Data) {
    if (channel != nullptr)
        *channel << Data;
    else
        buffer.insert(buffer.end(), Data.cbegin(), Data.cend());
}

void ItemBuffer::End() {
    if (channel != nullptr && !buffer.empty()) {
        *channel << buffer;
        buffer.resize(0);
    }
    if (!ended)
        master.Ended(*this);
    ended = true;
}


Item::Item(Encoder* E, ItemBuffer& B)
    : encoder(E), buffer(B)
{ }

Item::~Item() {
    buffer.End();
    delete encoder;
}

Item& Item::operator<<(Structure S) {
    std::vector<char>* buf = buffer.IntermediateBuffer();
    if (encoder->EncodeOutputsDirectly() && buf != nullptr) {
        // Writing directly to output buffer.
        encoder->Encode(*buf, S);
        return *this;
    }
    if (encoder->Encode(encode_buffer, S)) {
        buffer << encode_buffer;
        encode_buffer.resize(0);
    }
    return *this;
}

Item& Item::operator<<(const ValueReference& VR) {
    std::vector<char>* buf = buffer.IntermediateBuffer();
    if (encoder->EncodeOutputsDirectly() && buf != nullptr) {
        // Writing directly to output buffer.
        encoder->Encode(*buf, VR);
        return *this;
    }
    if (encoder->Encode(encode_buffer, VR)) {
        buffer << encode_buffer;
        encode_buffer.resize(0);
    }
    return *this;
}


void Output::AllocateChannels(bool SideChannel,
    std::vector<std::pair<OutputChannel*,ItemBuffer*> >& List)
{
    if (buffers.empty()) // Nothing to use the channel.
        return;
    // Find an available channel.
    std::pair<OutputChannel*,ItemBuffer*>* available = nullptr;
    for (size_t k = 0; k < List.size(); ++k)
        if (List[k].second == nullptr) {
            available = &List[k];
            break;
        }
    if (available == nullptr)
        return;
    // Get ended buffers out of the way first.
    for (size_t k = buffers.size(); k; --k) {
        ItemBuffer* current = buffers[k - 1];
        if (!current->Ended() || current->SideChannel() != SideChannel)
            continue;
        if (current->Size())
            current->SetChannel(available->first); // Writes out all.
        delete current;
        buffers[k - 1] = buffers.back();
        buffers.pop_back();
    }
    // Find buffer with most data.
    ItemBuffer* largest = nullptr;
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

Output::Output(const Encoder& Format, OutputChannel& Main)
    : encoder(Format)
{
    mains.push_back(&Main);
}

Output::~Output() {
    // Lock.
    // An opportunity to check for Items not yet done with.
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
        sides.push_back(std::make_pair<OutputChannel*,ItemBuffer*>(&OC, nullptr));
        AllocateChannels(true, sides);
    } else {
        mains.push_back(std::make_pair<OutputChannel*,ItemBuffer*>(&OC, nullptr));
        AllocateChannels(false, mains);
    }
}

Item* Output::Writable(bool SideChannel) {
    // Lock.
    ItemBuffer* buffer = new ItemBuffer(*this, SideChannel);
    buffers.push_back(buffer);
    AllocateChannels(SideChannel,
        (SideChannel && !sides.empty()) ? sides : mains);
    return new Item(encoder->CreateSame(), *buffer);
}

void Output::Ended(ItemBuffer& IB) {
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
