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
#include <vector>


class StorageDataSinkJSON : public StorageDataSink {
private:
    Storage& storage;
    std::vector<char> key;
    RawData value;

public:
    StorageDataSinkJSON(Storage& S);
    ~StorageDataSinkJSON();
    const char *const Format() const;
    void Input(RawData::Iterator& Start, RawData::Iterator& End);
    void End();
    void Discard(RawData::Iterator& Start, RawData::Iterator& End);
};


#endif /* StorageDataSinkJSON_hpp */
