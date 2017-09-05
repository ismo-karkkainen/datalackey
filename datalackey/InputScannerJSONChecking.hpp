//
//  InputScannerJSONChecking.hpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 30.5.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#ifndef InputScannerJSONChecking_hpp
#define InputScannerJSONChecking_hpp


#include "InputScanner.hpp"
#include "Structure.hpp"
#include <stack>


class InputScannerJSONChecking : public InputScanner {
private:
    // Stack for found structures. Individual values need not be identified
    // but one has to know if inside string etc.
    std::stack<Structure> open;
    bool in_string;
    bool escaping;
    bool bad_stream;

public:
    std::tuple<Recipient, RawData::Iterator, RawData::Iterator>
        scan_input(InputScanner::Recipient Previous,
            RawData::Iterator RangeBegin, RawData::Iterator RangeEnd);

public:
    InputScannerJSONChecking(
        InputChannel& IC, MessageHandler& MH, StorageDataSink& SDS);
    ~InputScannerJSONChecking();

    const char* const Format() const;
};


#endif /* InputScannerJSONChecking_hpp */
