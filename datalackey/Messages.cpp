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


CmdErrorArgumentSth::CmdErrorArgumentSth(
    const char* const Cmd, const char* const Sth)
    : cmd(Cmd), sth(Sth)
{ }

void CmdErrorArgumentSth::Report(Output& Out) const {
    Send(Out, Message::id);
}

void CmdErrorArgumentSth::Send(Output& Out, const SimpleValue& Id) const {
    message(Out, Id, cmd, "error", "argument", sth);
}


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


CmdErrorArgumentSthArg::CmdErrorArgumentSthArg(
    const char* const Cmd, const char* const Sth)
    : cmd(Cmd), sth(Sth)
{ }

void CmdErrorArgumentSthArg::Report(Output& Out) const {
    Send(Out, Message::id, Message::item);
}

void CmdErrorArgumentSthArg::Send(
    Output& Out, const SimpleValue& Id, const char* const Arg) const
{
    message(Out, Id, cmd, "error", "argument", sth, Arg);
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


ErrorCommandSth::ErrorCommandSth(const char* const Sth)
    : sth(Sth)
{ }

void ErrorCommandSth::Report(Output& Out) const {
    Send(Out, Message::id);
}

void ErrorCommandSth::Send( Output& Out, const SimpleValue& Id) const {
    message(Out, Id, "error", "command", sth);
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


CmdSthList::CmdSthList(const char* const Cmd, const char* const Sth)
    : cmd(Cmd), sth(Sth)
{ }

void CmdSthList::Report(Output& Out) const {
    std::vector<std::shared_ptr<SimpleValue>> dots;
    dots.push_back(Message::dots);
    Send(Out, Message::id, dots);
}

void CmdSthList::Send(Output& Out, const SimpleValue& Id,
    const std::vector<std::shared_ptr<SimpleValue>>& List) const
{
    listmessage(Out, Id, cmd, sth, List);
}

void CmdSthList::Send(Output& Out, const SimpleValue& Id,
    const std::vector<std::string>& List) const
{
    listmessage(Out, Id, cmd, sth, List);
}


NullNtfSthArg::NullNtfSthArg(
    const char* const Ntf, const char* const Sth, int PlaceholderCount)
    : ntf(Ntf), sth(Sth), placeholder_count(PlaceholderCount)
{
    assert(0 < placeholder_count && placeholder_count < 3);
}

void NullNtfSthArg::Report(Output& Out) const {
    if (placeholder_count == 1)
        Send(Out, Message::item);
    else
        Send(Out, Message::item, Message::item);
}

void NullNtfSthArg::Send(
    Output& Out, const char* const Arg, const char* const Arg2) const
{
    assert((Arg2 != nullptr && placeholder_count == 2) ||
        (Arg2 == nullptr && placeholder_count == 1));
    message(Out, ntf, sth, Arg, Arg2);
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


CmdSthArg2::CmdSthArg2(const char* const Cmd, const char* const Sth)
    : cmd(Cmd), sth(Sth)
{ }

void CmdSthArg2::Report(Output& Out) const {
    Send(Out, Message::id, Message::item, Message::item);
}

void CmdSthArg2::Send(Output& Out, const SimpleValue& Id,
    const char* const Arg, const char* const Arg2) const
{
    message(Out, Id, cmd, sth, Arg, Arg2);
}


Sth2Opt::Sth2Opt(const char* const Sth, const char* const Sth2,
    const char* const Opt)
    : sth(Sth), sth2(Sth2), opt(Opt)
{ }

void Sth2Opt::Report(Output& Out) const {
    Send(Out, Message::id);
}

void Sth2Opt::Send(Output& Out, const SimpleValue& Id) const {
    message(Out, Id, sth, sth2, opt);
}


Sth2List::Sth2List(const char* const Sth, const char* const Sth2)
    : sth(Sth), sth2(Sth2)
{ }

void Sth2List::Report(Output& Out) const {
    std::vector<std::string> dots;
    dots.push_back(Message::dots->String());
    Send(Out, Message::id, dots);
}

void Sth2List::Send(Output& Out, const SimpleValue& Id,
    const std::vector<std::string>& List) const
{
    listmessage(Out, Id, sth, sth2, List);
}


NullErrorSthOpt msg_null_error_format("format");
NullErrorSthOpt msg_error_identifier_missing("identifier", "missing");
NullErrorSthOpt msg_error_identifier_invalid("identifier", "invalid");
ErrorCommandSth msg_error_command_missing("missing");
ErrorCommandSth msg_error_command_not_string("not-string");
ErrorCommandSthArg msg_error_command_unknown("unknown");
ArgErrorArgumentSth msg_arg_error_argument_not_integer("not-integer");
ArgErrorArgumentSth msg_arg_error_argument_invalid("invalid");

NullNtfSthArg ntf_data_deleted("data", "deleted", 1);
NullNtfSthArg ntf_data_renamed("data", "renamed", 2);
NullNtfSthList ntf_data_stored("data", "stored");

Sth2Opt msg_channel_reset("channel", "reset");
Sth2Opt msg_run_error_format("run", "error", "format");
Sth2Opt msg_error_format("error", "format");

Sth2Opt msg_error_identifier_not_string("error", "identifier", "not-string");
Sth2List msg_data_stored("data", "stored");
