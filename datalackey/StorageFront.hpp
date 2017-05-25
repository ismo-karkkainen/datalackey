//
//  StorageFront.hpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 16.5.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#ifndef StorageFront_hpp
#define StorageFront_hpp

#include "InputScanner.hpp"



// Base class for input channels.
class StorageFront {
public:
    virtual ~StorageFront();
    virtual const char *const Format() const = 0;
    // Pass a data dictionary using this.
    virtual void Input(InputScanner::Iterator& Start, InputScanner::Iterator& End) = 0;
    virtual void End() = 0;
};


#endif /* StorageFront_hpp */
