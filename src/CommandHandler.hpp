//
//  CommandHandler.hpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 19.9.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//
// Licensed under Universal Permissive License. See License.txt.

#ifndef CommandHandler_hpp
#define CommandHandler_hpp

#include "MessageHandler.hpp"
#include "Command.hpp"
#include "Output.hpp"
#include "NumberValue.hpp"
#include <string>
#include <map>


class CommandHandler : public MessageHandler {
protected:
    std::map<std::string,Command*> handlers;
    Output& out;

public:
    CommandHandler(Output& Out);
    ~CommandHandler();

    // Ownership transfers.
    void AddCommand(Command* C);
};


#endif /* CommandHandler_hpp */
