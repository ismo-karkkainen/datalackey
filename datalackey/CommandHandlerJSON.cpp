//
//  CommandHandlerJSON.cpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 25.5.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#include "CommandHandlerJSON.hpp"
#include "Value_t.hpp"
#include "NumberValue.hpp"
#include "StringValue.hpp"
#include "NullValue.hpp"
#include <json.hpp>
#include <cmath>

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
        return error("format");
    }
    assert(cmd.is_array());
    if (cmd.size() == 0)
        return error("identifier", "missing");
    SimpleValue* identifier = nullptr;
    if (cmd[0].is_string())
        identifier = new StringValue(cmd[0].get<std::string>());
    else if (cmd[0].is_number()) {
        double d(cmd[0].get<double>()), i;
        if (0.0 != std::modf(d, &i))
            return error("identifier", "not-integer");
        identifier = new NumberValue(cmd[0].get<long long int>());
    } else if (cmd[0].is_null()) {
        identifier = new NullValue();
    } else
        return error("identifier", "not-string", "not-integer", "not-null");
    if (cmd.size() == 1)
        return error("command", "missing");
    if (!cmd[1].is_string())
        return error("command", "not-string");
    std::string command = cmd[1];
    auto iter = handlers.find(command);
    if (iter == handlers.end())
        return error(identifier, "command", "unknown");
    std::vector<SimpleValue*> args;
    for (size_t k = 2; k < cmd.size(); ++k) {
        if (cmd[k].is_string())
            args.push_back(new StringValue(cmd[k].get<std::string>()));
        else if (cmd[k].is_number()) {
            double d(cmd[k].get<double>()), i;
            if (0.0 != std::modf(d, &i))
                return error(identifier, "argument", "not-integer");
            args.push_back(new NumberValue(cmd[k].get<long long int>()));
        } else if (cmd[k].is_null()) {
            args.push_back(new NullValue());
        } else
            return error(identifier,
                "argument", "not-string", "not-integer", "not-null");
    }
    // Callee changes args item to nullptr if it takes ownership.
    iter->second->Perform(*identifier, args);
    for (auto item : args)
        delete item;
    delete identifier;
    return true;
}

void CommandHandlerJSON::Discard(
    RawData::ConstIterator& Start, RawData::ConstIterator& End)
{
    buffer.Clear();
}
