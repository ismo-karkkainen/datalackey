//
//  MessageHandler.hpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 16.5.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#ifndef MessageHandler_hpp
#define MessageHandler_hpp

#include "InputScanner.hpp"


// Base class for input channels.
class MessageHandler {
public:
    virtual ~MessageHandler();
    virtual const char *const Format() = 0;
    // Pass message array using this.
    virtual void Input(InputScanner::Iterator& Start, InputScanner::Iterator& End) = 0;
    virtual void End() = 0;
};


#endif /* MessageHandler_hpp */
