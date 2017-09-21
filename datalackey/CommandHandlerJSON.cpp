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
    : out(Out)
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
        return true;
    }
    std::vector<std::string> args;
    for (size_t k = 1; k < cmd.size(); ++k) {
        if (cmd[k].is_string())
            args.push_back(cmd[k]);
        else {
            // The output format may not be JSON so we can not dump the value
            // and pass it through as is. Hence treat as an error.
            OutputItem* writer = out.Writable();
            *writer << Structure::Array
                << ValueRef<std::string>("error")
                << ValueRef<std::string>("command")
                << ValueRef<std::string>("argument")
                << Structure::End;
            delete writer;
            return true;
        }
    }
    iter->second->Perform(args);
    return true;
}

void CommandHandlerJSON::Discard(
    RawData::ConstIterator& Start, RawData::ConstIterator& End)
{
    buffer.Clear();
}
