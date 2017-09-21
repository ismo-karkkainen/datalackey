//
//  DeleteCommand.cpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 21.9.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#include "DeleteCommand.hpp"
#include "Value_t.hpp"


DeleteCommand::DeleteCommand(const char *const Name, Output& Out, Storage& S)
    : Command(Name, Out), storage(S)
{ }

DeleteCommand::~DeleteCommand() {
}

void DeleteCommand::Perform(const std::vector<std::string>& Arguments) {
    // An array with output identifier and labels.
    if (Arguments.size() < 2) {
        OutputItem* writer = out.Writable();
        *writer << Array
            << ValueRef<std::string>("error")
            << ValueRef<std::string>(Arguments[0])
            << ValueRef<std::string>("argument")
            << ValueRef<std::string>("missing")
            << End;
        delete writer;
        return;
    }
    // Check if everything can be made available and if not, return an error.
    std::vector<std::string> unavailable;
    for (size_t k = 1; k < Arguments.size(); ++k) {
        if (!storage.Delete(Arguments[k]))
            unavailable.push_back(Arguments[k]);
    }
    if (!unavailable.empty()) {
        OutputItem* writer = out.Writable();
        *writer << Array
            << ValueRef<std::string>("error")
            << ValueRef<std::string>(Arguments[0])
            << ValueRef<std::string>("unavailable");
        for (auto s : unavailable)
            *writer << ValueRef<std::string>(s);
        *writer << End;
        delete writer;
        return;
    }
    OutputItem* writer = out.Writable();
    *writer << Array
        << ValueRef<std::string>(Arguments[0])
        << End;
    delete writer;
}
