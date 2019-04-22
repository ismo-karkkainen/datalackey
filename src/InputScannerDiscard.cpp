//
//  InputScannerDiscard.cpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 3.12.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//
// Licensed under Universal Permissive License. See License.txt.

#include "InputScannerDiscard.hpp"


std::tuple<InputScanner::Recipient, RawData::ConstIterator, RawData::ConstIterator>
InputScannerDiscard::scan_input(InputScanner::Recipient Previous,
    RawData::ConstIterator RangeBegin, RawData::ConstIterator RangeEnd)
{
    return std::make_tuple(InputScanner::Discard, RangeBegin, RangeEnd);
}

InputScannerDiscard::InputScannerDiscard(
    InputChannel& IC, MessageHandler& MH, StorageDataSink& SDS)
    : InputScanner(IC, MH, SDS)
{ }

InputScannerDiscard::~InputScannerDiscard() {
}

const char* const InputScannerDiscard::Format() const {
    return nullptr;
}
