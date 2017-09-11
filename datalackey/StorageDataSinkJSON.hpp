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
#include <vector>


class StorageDataSinkJSON : public StorageDataSink {
private:
    enum Part { InHead, InKey, InColon, InValue, InEnd, BadInput };
    Storage& storage;
    std::vector<char> key;
    RawData value;
    // Needed for keeping track of the contents.
    Part part;
    int open_dicts, open_arrays;
    bool in_string;
    bool escaping;

    void pass_to_storage();

public:
    StorageDataSinkJSON(Storage& S);
    ~StorageDataSinkJSON();
    const char *const Format() const;
    bool Input(RawData::Iterator& Start, RawData::Iterator& End);
    bool End();
    void Discard(RawData::Iterator& Start, RawData::Iterator& End);
};


#endif /* StorageDataSinkJSON_hpp */
