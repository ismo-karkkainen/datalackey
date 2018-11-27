//
//  MessageReporter.hpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 21.11.2018
//  Copyright © 2018 Ismo Kärkkäinen. All rights reserved.
//

#ifndef MessageReporter_hpp
#define MessageReporter_hpp


#include "Message.hpp"
#include "Output.hpp"
#include <set>


// Used for asking a report of all messages that can be sent.
class MessageReporter {
private:
    std::set<const Message*> registered;

    MessageReporter();

public:
    void Register(const Message* M);
    void Report(Output& Out) const;

    static MessageReporter& Get();
};


#endif /* MessageReporter_hpp */
