//
//  MessagePassThrough.hpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 25.5.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#ifndef MessagePassThrough_hpp
#define MessagePassThrough_hpp

#include "RawData.hpp"
#include "MessageHandler.hpp"
#include "Output.hpp"


// For passing message input to Output without conversion.
class MessagePassThrough : public MessageHandler {
private:
    Output& out;
    OutputItem* writer;

public:
    MessagePassThrough(Output& Out);
    ~MessagePassThrough();
    const char *const Format() const;
    void Input(RawData::Iterator& Start, RawData::Iterator& End);
    void End();
    void Discard(RawData::Iterator& Start, RawData::Iterator& End);
};


#endif /* MessagePassThrough_hpp */
