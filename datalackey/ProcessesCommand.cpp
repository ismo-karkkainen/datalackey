//
//  ProcessesCommand.cpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 18.10.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#include "ProcessesCommand.hpp"
#include "Number_t.hpp"
#include "Messages.hpp"
#include "NullValue.hpp"
#include "Value_t.hpp"
#include <memory>


ProcessesCommand::ProcessesCommand(const char *const Name, Output& Out, const Processes& P)
    : Command(Name, Out), processes(P),
    msg_reply(Name, ""), description(Name)
{ }

ProcessesCommand::~ProcessesCommand() {
}

void ProcessesCommand::Perform(
    const SimpleValue& Id, std::vector<std::shared_ptr<SimpleValue>>& Arguments)
{
    if (!description.Validate(out, Id, Arguments))
        return;
    auto results = processes.List();
    std::vector<std::tuple<std::string, unsigned long long int>> pairs;
    pairs.reserve(results.size());
    SimpleValue* id(nullptr);
    pid_t pid = 0;
    for (const auto& pair : results) {
        std::tie(id, pid) = pair;
        pairs.push_back(std::make_tuple(
            id->String(), static_cast<unsigned long long int>(pid)));
        delete id;
    }
    msg_reply.Send(out, Id, pairs);
}
