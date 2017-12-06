//
//  OutputChannel.hpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 18.4.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#ifndef OutputChannel_hpp
#define OutputChannel_hpp

#include "RawData.hpp"
#include <vector>


// Base class for output channels.
class OutputChannel {
public:
    virtual ~OutputChannel();
    virtual OutputChannel& operator<<(const RawData& Buffer) = 0;
    virtual void Write(
        RawData::ConstIterator& Start, RawData::ConstIterator& End) = 0;
    virtual void Flush() = 0;
    // Return true if previous write operation failed.
    virtual bool Failed() const = 0;
};


#endif /* OutputChannel_hpp */
