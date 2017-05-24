//
//  InputChannel.hpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 10.5.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#ifndef InputChannel_hpp
#define InputChannel_hpp

#include <vector>


// Base class for input channels.
class InputChannel {
public:
    virtual ~InputChannel();
    // Append any data there may be. Nothing if no data available.
    virtual streamsize Read(std::vector<char>& Buffer) = 0;
    // Return true if the channel has closed.
    virtual bool Ended() = 0;
};


#endif /* InputChannel_hpp */
