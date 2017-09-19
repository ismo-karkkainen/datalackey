//
//  CommandHandler.hpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 19.9.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#ifndef CommandHandler_hpp
#define CommandHandler_hpp

#include "MessageHandler.hpp"
#include "Command.hpp"
#include <string>
#include <map>


class CommandHandler : public MessageHandler {
protected:
    std::map<std::string,Command*> handlers;

public:
    CommandHandler();
    ~CommandHandler();

    // Ownership does not transfer. Best to destroy this object before these.
    void AddCommand(Command* C);
};


#endif /* CommandHandler_hpp */
