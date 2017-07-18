//
//  MessageHandler.hpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 16.5.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#ifndef MessageHandler_hpp
#define MessageHandler_hpp

#include "Iterator.hpp"


// Base class for input channels.
class MessageHandler {
public:
    virtual ~MessageHandler();
    virtual const char *const Format() const = 0;
    // Pass message array using this.
    virtual void Input(Iterator& Start, Iterator& End) = 0;
    // Indicate end of message item with this.
    virtual void End() = 0;
    // Error in message, throw away. Possible closing part passed.
    virtual void Discard(Iterator& Start, Iterator& End) = 0;
};


#endif /* MessageHandler_hpp */
