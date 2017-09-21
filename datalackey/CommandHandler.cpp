//
//  CommandHandler.cpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 19.9.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#include "CommandHandler.hpp"
#include "Value_t.hpp"
#include <cassert>


CommandHandler::CommandHandler() {
}

CommandHandler::~CommandHandler() {
}

void CommandHandler::AddCommand(Command* C) {
    auto iter = handlers.find(C->Name());
    assert(iter == handlers.end());
    handlers[C->Name()] = C;
}
