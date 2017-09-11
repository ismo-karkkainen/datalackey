//
//  InputScannerJSON.hpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 4.9.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#ifndef InputScannerJSON_hpp
#define InputScannerJSON_hpp


#include "InputScanner.hpp"


class InputScannerJSON : public InputScanner {
private:
    // Counter for open structures. One has to know if inside string etc.
    int open_something;
    bool in_string;
    bool escaping;

public:
    std::tuple<Recipient, RawData::Iterator, RawData::Iterator>
        scan_input(InputScanner::Recipient Previous,
            RawData::Iterator RangeBegin, RawData::Iterator RangeEnd);

public:
    InputScannerJSON(InputChannel& IC, MessageHandler& MH, StorageDataSink& SDS);
    ~InputScannerJSON();

    const char* const Format() const;
};


#endif /* InputScannerJSON_hpp */
