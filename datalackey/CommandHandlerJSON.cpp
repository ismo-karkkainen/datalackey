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
#include "Messages.hpp"
#include <nlohmann/json.hpp>
#include <cmath>
#include <vector>
#include <memory>

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
        msg_null_error_format.Send(out);
        return false;
    }
    assert(cmd.is_array());
    if (cmd.size() == 0) {
        msg_error_identifier_missing.Send(out);
        return true;
    }
    std::unique_ptr<SimpleValue> identifier = nullptr;
    if (cmd[0].is_string())
        identifier = std::unique_ptr<SimpleValue>(
            new StringValue(cmd[0].get<std::string>()));
    else if (cmd[0].is_number()) {
        double d(cmd[0].get<double>()), i;
        if (0.0 != std::modf(d, &i)) {
            msg_error_identifier_invalid.Send(out);
            return true;
        }
        identifier = std::unique_ptr<SimpleValue>(
            new NumberValue(cmd[0].get<long long int>()));
    } else if (cmd[0].is_null()) {
        identifier = std::unique_ptr<SimpleValue>(new NullValue());
    } else {
        msg_error_identifier_invalid.Send(out);
        return true;
    }
    if (cmd.size() == 1) {
        msg_error_command_missing.Send(out, *identifier.get());
        msg_done.Send(out, *identifier.get());
        return true;
    }
    if (!cmd[1].is_string()) {
        msg_error_command_not_string.Send(out, *identifier.get());
        msg_done.Send(out, *identifier.get());
        return true;
    }
    std::string command = cmd[1];
    auto iter = handlers.find(command);
    if (iter == handlers.end()) {
        msg_error_command_unknown.Send(out, *identifier.get(), command.c_str());
        msg_done.Send(out, *identifier.get());
        return true;
    }
    std::vector<std::shared_ptr<SimpleValue>> args;
    for (size_t k = 2; k < cmd.size(); ++k) {
        if (cmd[k].is_string())
            args.push_back(std::shared_ptr<SimpleValue>(
                new StringValue(cmd[k].get<std::string>())));
        else if (cmd[k].is_number()) {
            double d(cmd[k].get<double>()), i;
            if (0.0 != std::modf(d, &i)) {
                msg_arg_error_argument_not_integer.Send(
                    out, *identifier.get(), command.c_str());
                msg_done.Send(out, *identifier.get());
                return true;
            }
            args.push_back(std::shared_ptr<SimpleValue>(
                new NumberValue(cmd[k].get<long long int>())));
        } else if (cmd[k].is_null()) {
            args.push_back(std::shared_ptr<SimpleValue>(new NullValue()));
        } else {
            msg_arg_error_argument_invalid.Send(
                out, *identifier.get(), command.c_str());
            msg_done.Send(out, *identifier.get());
            return true;
        }
    }
    iter->second->Perform(*(identifier.get()), args);
    return true;
}

void CommandHandlerJSON::Discard(
    RawData::ConstIterator& Start, RawData::ConstIterator& End)
{
    buffer.Clear();
}
