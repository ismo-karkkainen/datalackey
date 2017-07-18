//
//  OutputChannel.hpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 18.4.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#ifndef OutputChannel_hpp
#define OutputChannel_hpp

#include "Iterator.hpp"
#include <vector>


// Base class for output channels.
class OutputChannel {
public:
    virtual ~OutputChannel();
    virtual OutputChannel& operator<<(const std::vector<char>& Buffer) = 0;
    virtual void Write(Iterator& Start, Iterator& End) = 0;
    virtual void Flush() = 0;
};


#endif /* OutputChannel_hpp */
