//
//  MessagePassThrough.hpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 25.5.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#ifndef MessagePassThrough_hpp
#define MessagePassThrough_hpp

#include "MessageHandler.hpp"
#include "Output.hpp"


// Base class for input channels.
class MessagePassThrough {
private:
    Output& out;
    OutputItem* writer;

public:
    MessagePassThrough(Output& Out);
    ~MessagePassThrough();
    const char *const Format() const;
    // Pass message array using this.
    void Input(InputScanner::Iterator& Start, InputScanner::Iterator& End);
    void End();
};


#endif /* MessagePassThrough_hpp */
