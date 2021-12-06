//
//  OutputChannel.hpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 18.4.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//
// Licensed under Universal Permissive License. See License.txt.

#ifndef OutputChannel_hpp
#define OutputChannel_hpp

#include "RawData.hpp"
#include <vector>
#include <cstddef>


// Base class for output channels.
class OutputChannel {
public:
    virtual ~OutputChannel();

    // Returns how much was written.
    virtual size_t Write(
        RawData::ConstIterator& Start, RawData::ConstIterator& End) = 0;
    virtual void Flush() = 0;
    virtual void Close() = 0;
    // Return true if previous write operation failed.
    virtual bool Failed() const = 0;
    virtual bool Closed() const = 0;
};


#endif /* OutputChannel_hpp */
