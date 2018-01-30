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
#include "ProcessInput.hpp"
#include <memory>


GetCommand::GetCommand(const char *const Name, Output& Out, Storage& S,
    const char *const Format)
: Command(Name, Out), storage(S), format(Format != nullptr ? Format : "")
{ }

GetCommand::~GetCommand() {
}

void GetCommand::Perform(
    const SimpleValue& Id, std::vector<std::shared_ptr<SimpleValue>>& Arguments)
{
    // An array with output identifier and labels.
    if (Arguments.empty()) {
        Error(out, Id, "argument", "missing");
        return;
    }
    // Filter out invalid values.
    std::vector<std::shared_ptr<SimpleValue>> invalid;
    std::vector<std::shared_ptr<ProcessInput>> results;
    for (auto arg : Arguments) {
        if (dynamic_cast<StringValue*>(arg.get()) == nullptr) {
            invalid.push_back(arg);
            continue;
        }
        results.push_back(
            std::shared_ptr<ProcessInput>(new ProcessInput(arg, arg)));
    }
    if (!invalid.empty())
        ListMessage(out, Id, "invalid", invalid);
    // Process without input can call this but it has no format so do nothing.
    // (In theory should "output" the data but there is no format to use.)
    if (format.empty())
        return;
    // Try to get the data.
    storage.Prepare(format.c_str(), results);
    // Report the ones that were not present.
    std::vector<std::shared_ptr<SimpleValue>> missing;
    for (auto res : results)
        if (!res->Data())
            missing.push_back(res->SharedLabel());
    if (!missing.empty())
        ListMessage(out, Id, "missing", missing);
    // Write out remaining results.
    std::unique_ptr<OutputItem> writer(out.Writable(IsNullValue(&Id)));
    *writer << Array; // Start message array.
    Feed(*writer, Id);
    *writer << Dictionary; // Start data dictionary.
    std::vector<std::shared_ptr<SimpleValue>> failed;
    for (size_t k = 0; k < results.size(); ++k) {
        std::shared_ptr<DataReader> data = results[k]->Data();
        if (!data)
            continue;
        Feed(*writer, *(results[k]->Name()));
        *writer << RawItem;
        // Failure after this messes up the whole thing.
        while (std::shared_ptr<const RawData> block = data->Read(1048576))
            writer->Write(block->CBegin(), block->CEnd());
        if (data->Error())
            failed.push_back(results[k]->SharedLabel());
        results[k] = nullptr;
    }
    *writer << End << End; // Close data dictionary and message array.
    // If there were any failures, report them but the chances of the controller
    // getting these in any sensible manner are slim, maybe with one-line JSON.
    if (!failed.empty())
        ListMessage(out, Id, "failed", failed);
}
