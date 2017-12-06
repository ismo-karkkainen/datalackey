//
//  InputScannerWrapMessage.hpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 3.12.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#ifndef InputScannerWrapMessage_hpp
#define InputScannerWrapMessage_hpp


#include "InputScanner.hpp"


class InputScannerWrapMessage : public InputScanner {
public:
    std::tuple<Recipient, RawData::ConstIterator, RawData::ConstIterator>
        scan_input(InputScanner::Recipient Previous,
            RawData::ConstIterator RangeBegin, RawData::ConstIterator RangeEnd);

public:
    InputScannerWrapMessage(
        InputChannel& IC, MessageHandler& MH, StorageDataSink& SDS);
    ~InputScannerWrapMessage();

    const char* const Format() const;
};


#endif /* InputScannerWrapMessage_hpp */
