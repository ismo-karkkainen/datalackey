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
#include "Output.hpp"
#include "NumberValue.hpp"
#include <string>
#include <map>


class CommandHandler : public MessageHandler {
protected:
    std::map<std::string,Command*> handlers;
    Output& out;

    // Returns false. Used on format error.
    bool error(const char *const one);
    // Return true. For content errors.
    bool error(const char *const one, const char *const two,
        const char *const three = nullptr, const char *const four = nullptr);
    // Deletes identifier due to the use as convenience function.
    bool error(SimpleValue* Id, const char *const one,
        const char *const two = nullptr, const char *const three = nullptr,
        const char *const four = nullptr);

public:
    CommandHandler(Output& Out);
    ~CommandHandler();

    // Ownership does not transfer. Best to destroy this object before these.
    void AddCommand(Command* C);
};


#endif /* CommandHandler_hpp */
