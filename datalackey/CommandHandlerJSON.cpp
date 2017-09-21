//
//  CommandHandlerJSON.cpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 25.5.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#include "CommandHandlerJSON.hpp"
#include "Value_t.hpp"
#include "json.hpp"

using json = nlohmann::json;


CommandHandlerJSON::CommandHandlerJSON(Output& Out)
    : CommandHandler(Out)
{ }

CommandHandlerJSON::~CommandHandlerJSON() {
}

const char *const CommandHandlerJSON::Format() const {
    return "JSON";
}

bool CommandHandlerJSON::Input(
    RawData::ConstIterator& Start, RawData::ConstIterator& End)
{
    buffer.Append(Start, End);
    return true;
}

bool CommandHandlerJSON::End() {
    if (buffer.Empty())
        return true;
    json cmd;
    try {
        cmd = json::parse(buffer.CBegin(), buffer.CEnd());
        buffer.Clear();
    }
    catch (const std::exception& e) {
        buffer.Clear();
        return error("Format");
    }
    assert(cmd.is_array());
    if (cmd.size() == 0)
        return error("command", "missing");
    if (cmd.size() == 1)
        return error("identifier", "missing");
    if (!cmd[0].is_string())
        return error("command", "not-string");
    if (!cmd[1].is_string())
        return error("identifier", "not-string");
    std::string command = cmd[0];
    std::string identifier = cmd[1];
    auto iter = handlers.find(command);
    if (iter == handlers.end())
        return error(identifier.c_str(), "command", "unknown");
    std::vector<std::string> args;
    for (size_t k = 1; k < cmd.size(); ++k) {
        if (cmd[k].is_string())
            args.push_back(cmd[k]);
        else
            return error(identifier.c_str(), "argument", "not-string");
    }
    iter->second->Perform(args);
    return true;
}

void CommandHandlerJSON::Discard(
    RawData::ConstIterator& Start, RawData::ConstIterator& End)
{
    buffer.Clear();
}
