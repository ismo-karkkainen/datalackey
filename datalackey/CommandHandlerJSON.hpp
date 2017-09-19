//
//  CommandHandlerJSON.hpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 5.9.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#ifndef CommandHandlerJSON_hpp
#define CommandHandlerJSON_hpp

#include "CommandHandler.hpp"
#include "RawData.hpp"
#include "Output.hpp"


class CommandHandlerJSON : public CommandHandler {
private:
    Output& out;
    RawData buffer;

public:
    CommandHandlerJSON(Output& Out);
    ~CommandHandlerJSON();
    const char *const Format() const;
    bool Input(RawData::ConstIterator& Start, RawData::ConstIterator& End);
    bool End();
    void Discard(RawData::ConstIterator& Start, RawData::ConstIterator& End);
};


#endif /* CommandHandlerJSON_hpp */
