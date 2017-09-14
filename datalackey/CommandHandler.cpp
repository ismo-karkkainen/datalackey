//
//  CommandHandler.cpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 25.5.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#include "CommandHandler.hpp"
#include "Value_t.hpp"
#include "json.hpp"

using json = nlohmann::json;


CommandHandler::CommandHandler(Output& Out)
    : out(Out)
{ }

CommandHandler::~CommandHandler() {
}

const char *const CommandHandler::Format() const {
    return "JSON";
}

bool CommandHandler::Input(
    RawData::ConstIterator& Start, RawData::ConstIterator& End)
{
    buffer.Append(Start, End);
    return true;
}

bool CommandHandler::End() {
    if (buffer.Empty())
        return true;
    json cmd;
    try {
        cmd = json::parse(buffer.CBegin(), buffer.CEnd());
        buffer.Clear();
    }
    catch (const std::exception& e) {
        buffer.Clear();
        OutputItem* writer = out.Writable();
        *writer << Structure::Array
            << ValueRef<std::string>("error")
            << ValueRef<std::string>("command")
            << ValueRef<std::string>("format")
            << Structure::End;
        delete writer;
        return false;
    }
    assert(cmd.is_array());
    std::string command = cmd[0];
    auto iter = handlers.find(command);
    if (iter == handlers.end()) {
        OutputItem* writer = out.Writable();
        *writer << Structure::Array
            << ValueRef<std::string>("error")
            << ValueRef<std::string>("command")
            << ValueRef<std::string>("unknown")
            << Structure::End;
        delete writer;
        return false;
    }
    iter->second->Perform(cmd);
    return true;
}

void CommandHandler::Discard(
    RawData::ConstIterator& Start, RawData::ConstIterator& End)
{
    buffer.Clear();
}

void CommandHandler::AddCommand(Command* C) {
    auto iter = handlers.find(C->Name());
    assert(iter == handlers.end());
    handlers[C->Name()] = C;
}
