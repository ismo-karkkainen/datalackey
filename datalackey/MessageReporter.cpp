//
//  MessageReporter.hpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 21.11.2018
//  Copyright © 2018 Ismo Kärkkäinen. All rights reserved.
//

#include "MessageReporter.hpp"
#include <cassert>


MessageReporter::MessageReporter() { }

void MessageReporter::Register(const Message* M) {
    auto result = registered.insert(M);
    assert(result.second);
}

void MessageReporter::Report(Output& Out) const {
    for (auto iter : registered)
        iter->Report(Out);
}

MessageReporter& MessageReporter::Get() {
    static MessageReporter reporter;
    return reporter;
}


