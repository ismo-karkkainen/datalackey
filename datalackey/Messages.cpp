//
//  Messages.cpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 21.11.2018
//  Copyright © 2018 Ismo Kärkkäinen. All rights reserved.
//

#include "Messages.hpp"
#include "StringValue.hpp"
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


NullableSth2Opt2List::NullableSth2Opt2List(
    const char* const Sth, const char* const Sth2)
    : sth(Sth), sth2(Sth2)
{ }

void NullableSth2Opt2List::Report(Output& Out) const {
    std::vector<std::string> dots;
    dots.push_back(Message::dots->String());
    Send(Out, &Message::id, dots);
    Send(Out, nullptr, dots);
}

void NullableSth2Opt2List::Send(Output& Out, const SimpleValue* Id,
    const std::vector<std::string>& List) const
{
    if (Id != nullptr)
        listmessage(Out, *Id, sth, sth2, List);
    else
        listmessage(Out, sth, sth2, List);
}


NullNtfSthList::NullNtfSthList(const char* const Ntf, const char* const Sth)
    : ntf(Ntf), sth(Sth)
{ }

void NullNtfSthList::Report(Output& Out) const {
    std::vector<std::shared_ptr<SimpleValue>> dots;
    dots.push_back(Message::dots);
    Send(Out, dots);
}

void NullNtfSthList::Send(Output& Out,
    const std::vector<std::shared_ptr<SimpleValue>>& List) const
{
    listmessage(Out, ntf, sth, List);
}

void NullNtfSthList::Send(Output& Out, const std::vector<std::string>& List)
    const
{
    listmessage(Out, ntf, sth, List);
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

NullNtfSthList ntf_data_deleted("data", "deleted");;
NullNtfSthList ntf_data_renamed("data", "renamed");
NullNtfSthList ntf_data_stored("data", "stored");

Sth2Opt3 msg_channel_reset("channel", "reset");
Sth2Opt3 msg_run_error_format("run", "error", "format");
NullableSth2Opt3 msg_error_format("error", "format");

NullableSth2Opt2List msg_data_stored("data", "stored");
