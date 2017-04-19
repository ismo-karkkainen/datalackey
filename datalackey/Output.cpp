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


Output::Output(const Encoder& Format, OutputChannel& Main)
    : encoder(Format)
{
    mains.push_back(&Main);
}

Output::~Output() {
    // An opportunity to check for Items not yet done with.
    // Though we could be exiting on exception or something.
    // And where exactly are we going to report? This is the output.
    // Write to file in current directory maybe?
    for (auto& buffer : buffers)
        delete buffer;
    // OutputChannels allocated and owned outside.
}

void Output::AddChannel(OutputChannel& OC, bool SideChannel) {
    if (SideChannel)
        sides.push_back(&OC);
    else
        mains.push_back(&OC);
}

Item* Output::Writable(bool SideChannel) {
    ItemBuffer* buffer = new ItemBuffer(*this, SideChannel);
    buffers.push_back(buffer);
    return new Item(encoder->CreateSame(), *buffer);
}

void Output::Ended(ItemBuffer& IB) {
    // Check if has channel and output pending.
    // Has to give channel to specific ItemBuffer or mark as free.
    // Marking as free allows for allocation to next one who needs but it
    // requires information from ItemBuffer that something needs channel.
    // Maybe allocate to ItemBuffer that has most pending output based on a
    // guess that it will be most in need.
    // What if nothing needs channel? Then new ItemBuffer should discover it.
    // In the meanwhile, something could gain data and needs to write it.
}
