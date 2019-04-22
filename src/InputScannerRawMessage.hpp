//
//  InputScannerRawMessage.hpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 3.12.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//
// Licensed under Universal Permissive License. See License.txt.

#ifndef InputScannerRawMessage_hpp
#define InputScannerRawMessage_hpp


#include "InputScanner.hpp"


class InputScannerRawMessage : public InputScanner {
public:
    std::tuple<Recipient, RawData::ConstIterator, RawData::ConstIterator>
        scan_input(InputScanner::Recipient Previous,
            RawData::ConstIterator RangeBegin, RawData::ConstIterator RangeEnd);

public:
    InputScannerRawMessage(
        InputChannel& IC, MessageHandler& MH, StorageDataSink& SDS);
    ~InputScannerRawMessage();

    const char* const Format() const;
};


#endif /* InputScannerRawMessage_hpp */
