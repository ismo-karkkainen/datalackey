//
//  MessageReporter.hpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 21.11.2018
//  Copyright © 2018 Ismo Kärkkäinen. All rights reserved.
//

#include "MessageReporter.hpp"


MessageReporter::MessageReporter() : registering(true) { }

void MessageReporter::Register(const Message* M) {
    if (registering)
        registered.push_back(M);
}

void MessageReporter::Report(Output& Out) const {
    for (auto iter : registered)
        iter->Report(Out);
}

MessageReporter& MessageReporter::Get() {
    static MessageReporter reporter;
    return reporter;
}
