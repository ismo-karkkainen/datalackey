//
//  MessageReporter.hpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 21.11.2018
//  Copyright © 2018 Ismo Kärkkäinen. All rights reserved.
//
// Licensed under Universal Permissive License. See License.txt.

#ifndef MessageReporter_hpp
#define MessageReporter_hpp


#include "Message.hpp"
#include "Output.hpp"
#include <vector>


// Used for asking a report of all messages that can be sent.
class MessageReporter {
private:
    std::vector<const Message*> registered;
    bool registering;

    MessageReporter();

public:
    void Register(const Message* M);
    void Report(Output& Out) const;
    void StopRegistrations() { registering = false; }

    static MessageReporter& Get();
};


#endif /* MessageReporter_hpp */
