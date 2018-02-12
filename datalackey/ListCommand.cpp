//
//  ListCommand.cpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 12.9.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#include "ListCommand.hpp"
#include "Value_t.hpp"
#include "Number_t.hpp"
#include "Notifications.hpp"
#include "NullValue.hpp"
#include <memory>


ListCommand::ListCommand(const char *const Name, Output& Out, const Storage& S)
    : Command(Name, Out), storage(S)
{ }

ListCommand::~ListCommand() {
}

void ListCommand::Perform(
    const SimpleValue& Id, std::vector<std::shared_ptr<SimpleValue>>& Arguments)
{
    // An array with output identifier that was given after the command.
    if (!Arguments.empty()) {
        Error(out, Id, "argument", "unexpected");
        return;
    }
    auto results = storage.List();
    std::vector<std::string> labels;
    labels.reserve(results.size());
    StringValue label("");
    std::string format;
    size_t size;
    for (size_t k = 0; k < results.size(); ++k) {
        std::tie(label, format, size) = results[k];
        format = label.String();
        if (labels.empty() || labels.back() != format)
            labels.push_back(format);
    }
    ListMessage(out, Id, nullptr, labels);
}
