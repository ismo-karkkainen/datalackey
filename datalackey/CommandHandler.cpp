//
//  CommandHandler.cpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 25.5.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#include "CommandHandler.hpp"
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

bool CommandHandler::Input(RawData::Iterator& Start, RawData::Iterator& End)
{
    buffer.Append(Start, End);
    return true;
}

bool CommandHandler::End() {
    if (buffer.Empty())
        return; // Probably got discarded.
    json cmd;
    try {
        cmd = json::parse(buffer.CBegin(), buffer.CEnd());
    }
    catch (const std::exception& e) {
        std::string err("[ \"error\", \"command\", \"format\" ]");
        OutputItem* writer = out.Writable();
        *writer << ValueRef<std::string>(err) << End;
        delete writer;
        return false;
    }
    assert(cmd.is_array());
    std::string command = cmd[0];
    auto iter = handlers.find(command);
    if (iter == handlers.end()) {
        std::string err("[ \"error\", \"command\", \"unknown\" ]");
        OutputItem* writer = out.Writable();
        *writer << ValueRef<std::string>(err) << End;
        delete writer;
        return false;
    }
    iter->second->Perform(cmd);
}

void CommandHandler::Discard(RawData::Iterator& Start, RawData::Iterator& End) {
    buffer.Clear();
}

void CommandHandler::AddCommand(Command* C) {
    auto iter = handler.find(C->Name());
    assert(iter == handler.end());
    handler[C->Name()] = C;
}
