//
//  InputScannerJSON.hpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 30.5.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#ifndef InputScannerJSON_hpp
#define InputScannerJSON_hpp


#include "InputScanner.hpp"
#include "Structure.hpp"
#include <stack>


class InputScannerJSON : public InputScanner {
private:
    // Stack for found structures. Individual values need not be identified
    // but one has to know if inside string etc.
    std::stack<Structure> open;
    bool in_string;
    bool escaping;
    bool bad_stream;

public:
    std::tuple<Recipient, Iterator, Iterator>
        scan_input(InputScanner::Recipient Previous, Iterator RangeBegin, Iterator RangeEnd);

public:
    InputScannerJSON(InputChannel& IC, MessageHandler& MH, StorageFront& SF);
    ~InputScannerJSON();

    const char* const Format() const;
};


#endif /* InputScannerJSON_hpp */
