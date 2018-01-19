//
//  StorageDataSinkJSON.hpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 26.5.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#ifndef StorageDataSinkJSON_hpp
#define StorageDataSinkJSON_hpp

#include "StorageDataSink.hpp"
#include "Storage.hpp"
#include "Structure.hpp"
#include "Output.hpp"
#include "StringValueMapper.hpp"
#include "DataGroup.hpp"
#include <vector>


class StorageDataSinkJSON : public StorageDataSink {
private:
    enum Part { InHead, InKey, InColon, InValue, InEnd, BadInput };
    Storage& storage;
    DataGroup* group;
    std::vector<char> key;
    RawData value; // For accumulating data before passing it to group.
    SimpleValue* identifier;
    Output& notifications;
    const StringValueMapper* renamer;
    // Needed for keeping track of the contents.
    Part part;
    int open_dicts, open_arrays;
    bool in_string;
    bool escaping;
    bool ignoring_item;

    bool start_item();
    void pass_item();
    void end_item();
    void end_group();
    bool error_format() const;

public:
    StorageDataSinkJSON(Storage& S, const SimpleValue* Id,
        Output& ProblemNotifications,
        const StringValueMapper* Renamer = nullptr);
    ~StorageDataSinkJSON();
    const char *const Format() const;
    bool Input(RawData::ConstIterator& Start, RawData::ConstIterator& End);
    bool End();
    void Discard(RawData::ConstIterator& Start, RawData::ConstIterator& End);
};


#endif /* StorageDataSinkJSON_hpp */
