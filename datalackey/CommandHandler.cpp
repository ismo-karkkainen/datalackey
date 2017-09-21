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


bool CommandHandler::error(const char *const one) {
    OutputItem* writer = out.Writable();
    *writer << Array
        << ValueRef<std::string>("error")
        << ValueRef<std::string>(one)
        << Structure::End;
    delete writer;
    return false;
}

bool CommandHandler::error(
    const char *const one, const char *const two, const char *const three)
{
    OutputItem* writer = out.Writable();
    *writer << Array
        << ValueRef<std::string>("error")
        << ValueRef<std::string>(one)
        << ValueRef<std::string>(two);
    if (three != nullptr)
        *writer << ValueRef<std::string>(three);
    *writer << Structure::End;
    delete writer;
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
