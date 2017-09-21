//
//  GetCommand.cpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 21.9.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#include "GetCommand.hpp"
#include "Value_t.hpp"


GetCommand::GetCommand(const char *const Name, Output& Out, Storage& S,
    const char *const Format)
    : Command(Name, Out), storage(S), format(Format)
{ }

GetCommand::~GetCommand() {
}

void GetCommand::Perform(const std::vector<std::string>& Arguments) {
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
    std::vector<std::string> ready, loading, unavailable;
    std::vector<std::shared_ptr<const RawData>> present;
    for (size_t k = 1; k < Arguments.size(); ++k) {
        auto rd = storage.ReadyData(Arguments[k], format.c_str());
        if (rd) {
            ready.push_back(Arguments[k]);
            present.push_back(rd);
        } else if (storage.Preload(Arguments[k], format.c_str()))
            loading.push_back(Arguments[k]);
        else
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
    *writer << Array; // Start message array.
    *writer << ValueRef<std::string>(Arguments[0]);
    *writer << Dictionary; // Start data dictionary.
    for (size_t k = 0; k < ready.size(); ++k) {
        *writer << ValueRef<std::string>(ready[k]) << RawItem;
        writer->Write(present[k]->CBegin(), present[k]->CEnd());
        present[k] = nullptr; // No longer needed here.
    }
    for (auto s : loading) {
        *writer << ValueRef<std::string>(s) << RawItem;
        auto data = storage.Data(s, format.c_str());
        writer->Write(data->CBegin(), data->CEnd());
    }
    *writer << End << End; // Close data dictionary and message array.
    delete writer;
}
