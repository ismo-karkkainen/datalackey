//
//  CommandHandler.cpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 19.9.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//
// Licensed under Universal Permissive License. See License.txt.

#include "CommandHandler.hpp"
#include <cassert>


CommandHandler::CommandHandler(Output& Out)
    : out(Out)
{ }

CommandHandler::~CommandHandler() {
    for (auto iter : handlers)
        delete iter.second;
}

void CommandHandler::AddCommand(Command* C) {
    assert(handlers.find(C->Name()) == handlers.end());
    handlers[C->Name()] = C;
}
