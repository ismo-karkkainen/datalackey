//
//  Messages.cpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 21.11.2018
//  Copyright © 2018 Ismo Kärkkäinen. All rights reserved.
//

#include "Messages.hpp"
#include "StringValue.hpp"
#include "ReportConstants.hpp"
#include <vector>
#include <memory>
#include <cassert>


ArgErrorArgumentSth::ArgErrorArgumentSth(const char* const Sth)
    : sth(Sth)
{ }

void ArgErrorArgumentSth::Report(Output& Out) const {
    Send(Out, Message::id, Message::item);
}

void ArgErrorArgumentSth::Send(Output& Out, const SimpleValue& Id,
    const char* const Arg) const
{
    message(Out, Id, Arg, "error", "argument", sth);
}


NullErrorSthOpt::NullErrorSthOpt(const char* const Sth, const char* const Opt)
    : sth(Sth), opt(Opt)
{ }

void NullErrorSthOpt::Report(Output& Out) const {
    Send(Out);
}

void NullErrorSthOpt::Send(Output& Out) const {
    message(Out, "error", sth, opt);
}


ErrorCommandSthArg::ErrorCommandSthArg(const char* const Sth)
    : sth(Sth)
{ }

void ErrorCommandSthArg::Report(Output& Out) const {
    Send(Out, Message::id, Message::item);
}

void ErrorCommandSthArg::Send(
    Output& Out, const SimpleValue& Id, const char* const Arg) const
{
    message(Out, Id, "error", "command", sth, Arg);
}


Sth2Opt2List::Sth2Opt2List(const char* const Sth, const char* const Sth2,
    const char* const Opt, const char* const Opt2)
    : sth(Sth), sth2(Sth2), opt(Opt), opt2(Opt2)
{ }

void Sth2Opt2List::Report(Output& Out) const {
    std::vector<std::shared_ptr<SimpleValue>> dots;
    dots.push_back(Message::dots);
    Send(Out, Message::id, dots);
}

void Sth2Opt2List::Send(Output& Out, const SimpleValue& Id,
    const std::vector<std::shared_ptr<SimpleValue>>& List) const
{
    listmessage(Out, Id, sth, sth2, List, opt, opt2);
}

void Sth2Opt2List::Send(Output& Out, const SimpleValue& Id,
    const std::vector<std::string>& List) const
{
    listmessage(Out, Id, sth, sth2, List, opt, opt2);
}


NullableSth2List::NullableSth2List(
    const char* const Sth, const char* const Sth2)
    : sth(Sth), sth2(Sth2)
{ }

void NullableSth2List::Report(Output& Out) const {
    std::vector<std::string> dots;
    dots.push_back(Message::dots->String());
    Send(Out, &Message::id, dots);
    Send(Out, nullptr, dots);
}

void NullableSth2List::Send(Output& Out, const SimpleValue* Id,
    const std::vector<std::string>& List) const
{
    if (Id != nullptr)
        listmessage(Out, *Id, sth, sth2, List);
    else
        listmessage(Out, sth, sth2, List);
}


NullableSth2PairMap::NullableSth2PairMap(
    const char* const Sth, const char* const Sth2)
    : sth(Sth), sth2(Sth2)
{ }

void NullableSth2PairMap::Report(Output& Out) const {
    std::vector<std::tuple<std::string, long long int>> map;
    map.push_back(std::make_tuple(Message::id.String(), 0ULL));
    Send(Out, &Message::id, map);
    Send(Out, nullptr, map);
}

void NullableSth2PairMap::Send(Output& Out, const SimpleValue* Id,
    const std::vector<std::tuple<std::string, long long int>>& Map,
    bool ConvertZeroToNull) const
{
    mapmessage(Out, Id, sth, sth2, Map, ConvertZeroToNull);
}


NullNtfSthArgLL::NullNtfSthArgLL(const char* const Ntf, const char* const Sth)
    : ntf(Ntf), sth(Sth)
{ }

void NullNtfSthArgLL::Report(Output& Out) const {
    Send(Out, report::string.c_str(), 0LL);
}

void NullNtfSthArgLL::Send(Output& Out,
    const char* const Arg, long long int LL) const
{
    msg(Out, nullptr, ntf, sth, Arg, nullptr, nullptr, nullptr, nullptr, &LL);
}


Sth2Opt3::Sth2Opt3(const char* const Sth, const char* const Sth2,
    const char* const Opt, const char* const Opt2, const char* const Opt3)
    : sth(Sth), sth2(Sth2), opt(Opt), opt2(Opt2), opt3(Opt3)
{ }

void Sth2Opt3::Report(Output& Out) const {
    Send(Out, Message::id);
}

void Sth2Opt3::Send(Output& Out, const SimpleValue& Id) const {
    message(Out, Id, sth, sth2, opt, opt2, opt3);
}


NullableSth2Opt3::NullableSth2Opt3(
    const char* const Sth, const char* const Sth2,
    const char* const Opt, const char* const Opt2, const char* const Opt3)
    : sth(Sth), sth2(Sth2), opt(Opt), opt2(Opt2), opt3(Opt3)
{ }

void NullableSth2Opt3::Report(Output& Out) const {
    Send(Out, &Message::id);
    Send(Out, nullptr);
}

void NullableSth2Opt3::Send(Output& Out, const SimpleValue* Id) const {
    message(Out, Id, sth, sth2, opt, opt2, opt3);
}


NullErrorSthOpt msg_null_error_format("format");
NullErrorSthOpt msg_error_identifier_missing("identifier", "missing");
NullErrorSthOpt msg_error_identifier_invalid("identifier", "invalid");
ErrorCommandSthArg msg_error_command_missing("missing");
ErrorCommandSthArg msg_error_command_not_string("not-string");
ErrorCommandSthArg msg_error_command_unknown("unknown");
ArgErrorArgumentSth msg_arg_error_argument_not_integer("not-integer");
ArgErrorArgumentSth msg_arg_error_argument_invalid("invalid");

NullNtfSthArgLL ntf_data_deleted("data", "deleted");
NullNtfSthArgLL ntf_data_stored("data", "stored");
NullNtfSthArgLL ntf_data_error("data", "error");

Sth2Opt3 msg_channel_reset("channel", "reset");
Sth2Opt3 msg_run_error_format("run", "error", "format");
NullableSth2Opt3 msg_error_format("error", "format");

Sth2Opt3 msg_done("done", "");
