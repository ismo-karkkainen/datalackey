//
//  InputScannerRawMessage.cpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 3.12.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#include "InputScannerRawMessage.hpp"


std::tuple<InputScanner::Recipient, RawData::ConstIterator, RawData::ConstIterator>
InputScannerRawMessage::scan_input(InputScanner::Recipient Previous,
    RawData::ConstIterator RangeBegin, RawData::ConstIterator RangeEnd)
{
    return std::make_tuple(InputScanner::MessageEnd, RangeBegin, RangeEnd);
}

InputScannerRawMessage::InputScannerRawMessage(
    InputChannel& IC, MessageHandler& MH, StorageDataSink& SDS)
    : InputScanner(IC, MH, SDS)
{ }

InputScannerRawMessage::~InputScannerRawMessage() {
}

const char* const InputScannerRawMessage::Format() const {
    return nullptr;
}
