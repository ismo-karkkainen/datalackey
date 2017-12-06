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
#include "NullValue.hpp"


GetCommand::GetCommand(const char *const Name, Output& Out, Storage& S,
    const char *const Format)
    : Command(Name, Out), storage(S), format(Format)
{ }

GetCommand::~GetCommand() {
}

void GetCommand::Perform(
    const SimpleValue& Id, std::vector<SimpleValue*>& Arguments)
{
    // An array with output identifier and labels.
    if (Arguments.empty()) {
        Error(out, Id, "argument", "missing");
        return;
    }
    // Check if everything can be made available and if not, return an error.
    std::vector<StringValue*> ready, loading, unavailable;
    std::vector<SimpleValue*> not_string;
    std::vector<std::shared_ptr<const RawData>> present;
    for (auto arg : Arguments) {
        StringValue* label = dynamic_cast<StringValue*>(arg);
        if (label == nullptr) {
            not_string.push_back(arg);
            continue;
        }
        auto rd = storage.ReadyData(*label, format.c_str());
        if (rd) {
            ready.push_back(label);
            present.push_back(rd);
        } else if (storage.Preload(*label, format.c_str()))
            loading.push_back(label);
        else
            unavailable.push_back(label);
    }
    if (!not_string.empty()) {
        OutputItem* writer = out.Writable(IsNullValue(&Id));
        *writer << Array;
        Feed(*writer, Id);
        *writer << ValueRef<std::string>("error")
            << ValueRef<std::string>("argument")
            << ValueRef<std::string>("not-string");
        for (auto s : unavailable) {
            Feed(*writer, *s);
            delete s;
        }
        *writer << End;
        delete writer;
    }
    if (!unavailable.empty()) {
        OutputItem* writer = out.Writable(IsNullValue(&Id));
        *writer << Array;
        Feed(*writer, Id);
        *writer << ValueRef<std::string>("error")
            << ValueRef<std::string>("argument")
            << ValueRef<std::string>("unavailable");
        for (auto s : unavailable) {
            Feed(*writer, *s);
            delete s;
        }
        *writer << End;
        delete writer;
    }
    OutputItem* writer = out.Writable(IsNullValue(&Id));
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
