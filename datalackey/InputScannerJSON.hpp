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
    Output& notifications;
    const SimpleValue* identifier;

public:
    std::tuple<Recipient, RawData::ConstIterator, RawData::ConstIterator>
        scan_input(InputScanner::Recipient Previous,
            RawData::ConstIterator RangeBegin, RawData::ConstIterator RangeEnd);

public:
    InputScannerJSON(InputChannel& IC, MessageHandler& MH, StorageDataSink& SDS,
        Output& ProblemNotifications, const SimpleValue* Id = nullptr);
    ~InputScannerJSON();

    const char* const Format() const;
};


#endif /* InputScannerJSON_hpp */
