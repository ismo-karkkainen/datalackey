//
//  InputScannerDiscard.hpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 3.12.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#ifndef InputScannerDiscard_hpp
#define InputScannerDiscard_hpp


#include "InputScanner.hpp"


class InputScannerDiscard : public InputScanner {
public:
    std::tuple<Recipient, RawData::ConstIterator, RawData::ConstIterator>
        scan_input(InputScanner::Recipient Previous,
            RawData::ConstIterator RangeBegin, RawData::ConstIterator RangeEnd);

public:
    InputScannerDiscard(
        InputChannel& IC, MessageHandler& MH, StorageDataSink& SDS);
    ~InputScannerDiscard();

    const char* const Format() const;
};


#endif /* InputScannerDiscard_hpp */
