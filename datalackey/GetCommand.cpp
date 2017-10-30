//
//  GetCommand.cpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 21.9.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#include "GetCommand.hpp"
#include "Value_t.hpp"
#include "Notifications.hpp"
#include <cassert>


GetCommand::GetCommand(const char *const Name, Output& Out, Storage& S,
    const char *const Format)
    : Command(Name, Out), storage(S), format(Format)
{ }

GetCommand::~GetCommand() {
}

bool GetCommand::LabelsOnly() const {
    return true;
}

void GetCommand::Perform(
    const Identifier& Id, std::vector<SimpleValue*>& Arguments)
{
    // An array with output identifier and labels.
    if (Arguments.empty()) {
        Error(out, Id, "argument", "missing");
        return;
    }
    // Check if everything can be made available and if not, return an error.
    std::vector<Label*> ready, loading, unavailable;
    std::vector<std::shared_ptr<const RawData>> present;
    for (auto arg : Arguments) {
        Label* label = dynamic_cast<Label*>(arg);
        assert(label != nullptr);
        auto rd = storage.ReadyData(*label, format.c_str());
        if (rd) {
            ready.push_back(label);
            present.push_back(rd);
        } else if (storage.Preload(*label, format.c_str()))
            loading.push_back(label);
        else
            unavailable.push_back(label);
    }
    if (!unavailable.empty()) {
        OutputItem* writer = out.Writable();
        *writer << Array;
        Feed(*writer, Id);
        *writer << ValueRef<std::string>("error")
            << ValueRef<std::string>("unavailable");
        for (auto s : unavailable) {
            Feed(*writer, *s);
            delete s;
        }
        *writer << End;
        delete writer;
    }
    OutputItem* writer = out.Writable();
    *writer << Array; // Start message array.
    Feed(*writer, Id);
    *writer << Dictionary; // Start data dictionary.
    for (size_t k = 0; k < ready.size(); ++k) {
        Feed(*writer, *ready[k]);
        *writer << RawItem;
        writer->Write(present[k]->CBegin(), present[k]->CEnd());
        present[k] = nullptr; // No longer needed here.
    }
    for (auto s : loading) {
        Feed(*writer, *s);
        *writer << RawItem;
        auto data = storage.Data(*s, format.c_str());
        writer->Write(data->CBegin(), data->CEnd());
    }
    *writer << End << End; // Close data dictionary and message array.
    delete writer;
}
