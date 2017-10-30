//
//  CommandHandler.cpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 19.9.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#include "CommandHandler.hpp"
#include "Notifications.hpp"
#include <cassert>


bool CommandHandler::error(const char *const one) {
    Error(out, one);
    return false;
}

bool CommandHandler::error(
    const char *const one, const char *const two, const char *const three)
{
    Error(out, one, two, three);
    return true;
}

bool CommandHandler::error(Identifier* Id,
    const char *const one, const char *const two, const char *const three)
{
    Error(out, *Id, one, two, three);
    delete Id; // For convenience since caller is returning on this call.
    return true;
}

CommandHandler::CommandHandler(Output& Out)
    : out(Out)
{ }

CommandHandler::~CommandHandler() {
}

void CommandHandler::AddCommand(Command* C) {
    auto iter = handlers.find(C->Name());
    assert(iter == handlers.end());
    handlers[C->Name()] = C;
}
