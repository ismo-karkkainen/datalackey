//
//  StorageDataSink.hpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 16.5.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#ifndef StorageDataSink_hpp
#define StorageDataSink_hpp

#include "RawData.hpp"


// Base class for passing data to storage.
class StorageDataSink {
public:
    virtual ~StorageDataSink();
    virtual const char *const Format() const = 0;
    // Pass a data dictionary using this.
    virtual bool Input(
        RawData::ConstIterator& Start, RawData::ConstIterator& End) = 0;
    virtual bool End() = 0;
    // Error in data, throw away. Range may contain closing data.
    virtual void Discard(
        RawData::ConstIterator& Start, RawData::ConstIterator& End) = 0;
};


#endif /* StorageDataSink_hpp */
