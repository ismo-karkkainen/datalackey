//
//  CommandDescription.cpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 4.12.2018
//  Copyright © 2018 Ismo Kärkkäinen. All rights reserved.
//
// Licensed under Universal Permissive License. See License.txt.

#include "CommandDescription.hpp"
#include "CommandReporter.hpp"
#include "ReportConstants.hpp"
#include <cassert>


Sth2Opt2List CommandDescription::error_missing("error", "missing");
Sth2Opt2List CommandDescription::error_unexpected("error", "unexpected");
Sth2Opt2List CommandDescription::error_not_string("error", "not-string");
Sth2Opt2List CommandDescription::error_not_string_null(
    "error", "not-string-null");
Sth2Opt2List CommandDescription::error_unknown("error", "unknown");
Sth2Opt2List CommandDescription::error_pairless("error", "pairless");


OutputItem* CommandDescription::array_id_name(OutputItem* Writer) const {
    *Writer << Structure::Array
        << ValueRef<std::string>(report::id)
        << ValueRef<std::string>(name);
    return Writer;
}

bool CommandDescription::error(Sth2Opt2List& Msg,
    Output* Out, const SimpleValue& Id,
    const std::vector<std::shared_ptr<SimpleValue>>& Arguments,
    size_t PastLastIndex)
{
    assert(Out != nullptr);
    return error(Msg, *Out, Id, Arguments, PastLastIndex);
}

bool CommandDescription::error(Sth2Opt2List& Msg,
    Output& Out, const SimpleValue& Id,
    const std::vector<std::shared_ptr<SimpleValue>>& Arguments,
    size_t PastLastIndex)
{
    assert(PastLastIndex <= Arguments.size());
    std::vector<std::shared_ptr<SimpleValue>> wanted;
    wanted.reserve(2 + PastLastIndex);
    wanted.push_back(std::shared_ptr<SimpleValue>(Id.Clone()));
    wanted.push_back(std::shared_ptr<SimpleValue>(new StringValue(name)));
    for (size_t k = 0; k < PastLastIndex; ++k)
        wanted.push_back(Arguments[k]);
    Msg.Send(Out, Id, wanted);
    return false;
}

CommandDescription::CommandDescription(const char* const Name)
    : name(Name)
{
    CommandReporter::Get().Register(this);
}

CommandDescription::~CommandDescription() { }

bool CommandDescription::Validate(Output& Out, const SimpleValue& Id,
    std::vector<std::shared_ptr<SimpleValue>>& Arguments)
{
    if (validate(Out, Id, Arguments))
        return true;
    msg_done.Send(Out, Id);
    return false;
}
