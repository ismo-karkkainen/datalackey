//
//  InputScannerWrapMessage.cpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 3.12.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#include "InputScannerWrapMessage.hpp"


std::tuple<InputScanner::Recipient, RawData::ConstIterator, RawData::ConstIterator>
InputScannerWrapMessage::scan_input(InputScanner::Recipient Previous,
    RawData::ConstIterator RangeBegin, RawData::ConstIterator RangeEnd)
{
    return std::make_tuple(InputScanner::MessageEnd, RangeBegin, RangeEnd);
}

InputScannerWrapMessage::InputScannerWrapMessage(
    InputChannel& IC, MessageHandler& MH, StorageDataSink& SDS)
    : InputScanner(IC, MH, SDS)
{ }

InputScannerWrapMessage::~InputScannerWrapMessage() {
}

const char* const InputScannerWrapMessage::Format() const {
    return nullptr;
}
