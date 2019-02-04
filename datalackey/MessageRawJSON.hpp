//
//  MessageRawJSON.hpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 3.12.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//
// Licensed under Universal Permissive License. See License.txt.

#ifndef MessageRawJSON_hpp
#define MessageRawJSON_hpp

#include "RawData.hpp"
#include "MessageHandler.hpp"
#include "Output.hpp"


// For passing message input to Output without conversion.
class MessageRawJSON : public MessageHandler {
private:
    Output& out;
    OutputItem* writer;
    const SimpleValue& identifier;

public:
    MessageRawJSON(Output& Out, const SimpleValue& Identifier);
    ~MessageRawJSON();
    const char *const Format() const;
    bool Input(RawData::ConstIterator& Start, RawData::ConstIterator& End);
    bool End();
    void Discard(RawData::ConstIterator& Start, RawData::ConstIterator& End);
};


#endif /* MessageRawJSON_hpp */
