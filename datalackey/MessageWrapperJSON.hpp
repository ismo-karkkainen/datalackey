//
//  MessageWrapperJSON.hpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 3.12.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#ifndef MessageWrapperJSON_hpp
#define MessageWrapperJSON_hpp

#include "RawData.hpp"
#include "MessageHandler.hpp"
#include "Output.hpp"


// For passing message input to Output without conversion.
class MessageWrapperJSON : public MessageHandler {
private:
    Output& out;
    OutputItem* writer;
    const SimpleValue* identifier;

public:
    MessageWrapperJSON(Output& Out, const SimpleValue* Identifier);
    ~MessageWrapperJSON();
    const char *const Format() const;
    bool Input(RawData::ConstIterator& Start, RawData::ConstIterator& End);
    bool End();
    void Discard(RawData::ConstIterator& Start, RawData::ConstIterator& End);
};


#endif /* MessageWrapperJSON_hpp */
