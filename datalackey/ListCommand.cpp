//
//  ListCommand.cpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 12.9.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#include "ListCommand.hpp"
#include "Value_t.hpp"


ListCommand::ListCommand(const char *const Name, Output& Out, const Storage& S)
    : Command(Name, Out), storage(S)
{ }

ListCommand::~ListCommand() {
}

void ListCommand::Perform(nlohmann::json& JSONCommand) {
    // An array with output identifier that was given after the command.
    if (!JSONCommand.is_array() || JSONCommand.size() != 2) {
        OutputItem* writer = out.Writable();
        *writer << Structure::Array
            << ValueRef<std::string>("error")
            << ValueRef<std::string>("command")
            << ValueRef<std::string>("argument")
            << Structure::End;
        delete writer;
        return;
    }
    auto results = storage.List();
    OutputItem* writer = out.Writable();
    *writer << Array; // Start message array.
    // This by-passes normal encoding in writer so take care of separator, too.
    std::string id = JSONCommand[1].dump();
    id += ",";
    RawData tmp;
    tmp.Append(id.c_str());
    RawData::ConstIterator cb(tmp.CBegin()), ce(tmp.CEnd());
    writer->Write(cb, ce);
    *writer << Dictionary; // Start label dictionary.
    std::string label, format;
    size_t size;
    if (!results.empty()) {
        std::tie(label, format, size) = results[0];
        *writer << ValueRef<std::string>(label) << Dictionary
            << ValueRef<std::string>(format) << ValueRef<size_t>(size);
        std::string previous(label);
        for (size_t k = 1; k < results.size(); ++k) {
            std::tie(label, format, size) = results[k];
            if (previous != label) {
                // Close previous format:size dictionary and start new.
                *writer << End << ValueRef<std::string>(label) << Dictionary;
                previous = label;
            }
            *writer << ValueRef<std::string>(format) << ValueRef<size_t>(size);
        }
        *writer << End; // Close previous format:size dictionary.
    }
    *writer << End << End; // Close label dictionary and message array.
    delete writer;
}

void ListCommand::Help() {
}
