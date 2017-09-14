//
//  CommandHandler.hpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 5.9.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#ifndef CommandHandler_hpp
#define CommandHandler_hpp

#include "MessageHandler.hpp"
#include "RawData.hpp"
#include "Output.hpp"
#include "Command.hpp"
#include <map>
#include <string>


// For passing message input to Output without conversion.
class CommandHandler : public MessageHandler {
private:
    Output& out;
    RawData buffer;

    std::map<std::string,Command*> handlers;

public:
    CommandHandler(Output& Out);
    ~CommandHandler();
    const char *const Format() const;
    bool Input(RawData::ConstIterator& Start, RawData::ConstIterator& End);
    bool End();
    void Discard(RawData::ConstIterator& Start, RawData::ConstIterator& End);

    // Ownership does not transfer. Best to destroy CommandHandler before these.
    void AddCommand(Command* C);
};


#endif /* CommandHandler_hpp */
