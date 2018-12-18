//
//  GetCommand.cpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 21.9.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#include "GetCommand.hpp"
#include "Value_t.hpp"
#include "Messages.hpp"
#include "NullValue.hpp"
#include "ProcessInput.hpp"
#include <memory>


GetCommand::GetCommand(const char *const Name, Output& Out, Storage& S)
    : Command(Name, Out), storage(S),
    msg_missing(Name, "missing"),
    msg_failed(Name, "failed"),
    msg_reply(Name, "", "mapping label-to-value"),
    description(Name)
{ }

GetCommand::~GetCommand() {
}

void GetCommand::Perform(
    const SimpleValue& Id, std::vector<std::shared_ptr<SimpleValue>>& Arguments)
{
    if (!description.Validate(out, Id, Arguments))
        return;
    // Process without input can call this but there is no format in that case.
    if (out.Format() == nullptr)
        return;
    std::vector<std::shared_ptr<ProcessInput>> results;
    for (auto& arg : Arguments)
        results.push_back(
            std::shared_ptr<ProcessInput>(new ProcessInput(arg, arg)));
    // Try to get the data.
    storage.Prepare(out.Format(), results);
    // Report the ones that were not present.
    std::vector<std::shared_ptr<SimpleValue>> missing;
    for (auto& res : results)
        if (!res->Data())
            missing.push_back(res->SharedLabel());
    if (!missing.empty())
        msg_missing.Send(out, Id, missing);
    // Write out remaining results.
    std::unique_ptr<OutputItem> writer(out.Writable(IsNullValue(&Id)));
    *writer << Array; // Start message array.
    Message::Feed(*writer, Id);
    *writer << ValueRef<std::string>(Name())
        << ValueRef<std::string>("")
        << Dictionary; // Start data dictionary.
    std::vector<std::shared_ptr<SimpleValue>> failed;
    for (size_t k = 0; k < results.size(); ++k) {
        std::shared_ptr<DataReader> data = results[k]->Data();
        if (!data)
            continue;
        Message::Feed(*writer, *(results[k]->Name()));
        *writer << RawItem;
        // Failure after this messes up the whole thing.
        while (std::shared_ptr<const RawData> block = data->Read(1048576))
            writer->Write(block->CBegin(), block->CEnd());
        if (data->Error())
            failed.push_back(results[k]->SharedLabel());
        results[k] = nullptr;
    }
    *writer << End << End; // Close data dictionary and message array.
    writer.reset();
    // If there were any failures, report them but the chances of the controller
    // getting these in any sensible manner are slim. Maybe with one-line JSON.
    if (!failed.empty())
        msg_failed.Send(out, Id, failed);
    msg_done.Send(out, Id);
}
