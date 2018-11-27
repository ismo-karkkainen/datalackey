//
//  ProcessMessages.cpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 26.11.2018
//  Copyright © 2018 Ismo Kärkkäinen. All rights reserved.
//

#include "ProcessMessages.hpp"
#include <cassert>


NullNtfSthIdInt::NullNtfSthIdInt(const char* const Ntf, const char* const Sth)
    : ntf(Ntf), sth(Sth)
{ }

void NullNtfSthIdInt::Report(Output& Out) const {
    Send(Out, Message::id, 0);
}

void NullNtfSthIdInt::Send(Output& Out, const SimpleValue& Id, int Int) const {
    message(Out, ntf, sth, Id, Int);
}


CmdSthInt::CmdSthInt(const char* const Cmd, const char* const Sth)
    : cmd(Cmd), sth(Sth)
{ }

void CmdSthInt::Report(Output& Out) const {
    Send(Out, Message::id, 0);
}

void CmdSthInt::Send(Output& Out, const SimpleValue& Id, int Int) const {
    message(Out, Id, cmd, sth, Int);
}


CmdSthOpt3::CmdSthOpt3(const char* const Cmd, const char* const Sth,
    const char* const Opt, const char* const Opt2, const char* const Opt3)
    : cmd(Cmd), sth(Sth), opt(Opt), opt2(Opt2), opt3(Opt3)
{ }

void CmdSthOpt3::Report(Output& Out) const {
    Send(Out, Message::id);
}

void CmdSthOpt3::Send(Output& Out, const SimpleValue& Id) const {
    message(Out, Id, cmd, sth, opt, opt2, opt3);
}


CmdErrorArgArgumentSth::CmdErrorArgArgumentSth(
    const char* const Cmd, const char* const Sth)
    : cmd(Cmd), sth(Sth)
{ }

void CmdErrorArgArgumentSth::Report(Output& Out) const {
    Send(Out, Message::id, Message::item);
}

void CmdErrorArgArgumentSth::Send(Output& Out, const SimpleValue& Id,
    const char* const Arg) const
{
    message(Out, Id, cmd, "error", Arg, "argument", sth);
}


CmdErrorArgDuplicateArg::CmdErrorArgDuplicateArg(const char* const Cmd)
    : cmd(Cmd)
{ }

void CmdErrorArgDuplicateArg::Report(Output& Out) const {
    Send(Out, Message::id, Message::item, Message::item);
}

void CmdErrorArgDuplicateArg::Send(Output& Out, const SimpleValue& Id,
    const char* const Arg, const char* const Arg2) const
{
    message(Out, Id, cmd, "error", Arg, "duplicate", Arg2);
}


RunErrorSth2OptArg::RunErrorSth2OptArg(
    const char* const Sth, const char* const Sth2, const char* const Opt)
    : sth(Sth), sth2(Sth2), opt(Opt)
{ }

void RunErrorSth2OptArg::Report(Output& Out) const {
    Send(Out, Message::id, Message::item);
}

void RunErrorSth2OptArg::Send(
    Output& Out, const SimpleValue& Id, const char* const Arg) const
{
    if (opt == nullptr)
        message(Out, Id, "run", "error", sth, sth2, Arg);
    else
        message(Out, Id, "run", "error", sth, sth2, opt, Arg);
}


RunErrorSthArg2::RunErrorSthArg2(const char* const Sth, int ArgCount)
    : sth(Sth), arg_count(ArgCount)
{
    assert(0 < ArgCount && ArgCount < 3);
}

void RunErrorSthArg2::Report(Output& Out) const {
    Send(Out, Message::id, Message::item,
        (arg_count > 1) ? Message::item : nullptr);
}

void RunErrorSthArg2::Send(Output& Out, const SimpleValue& Id,
    const char* const Arg, const char* const Arg2) const
{
    assert((arg_count == 1 && Arg2 == nullptr) ||
        (arg_count == 2 && Arg2 != nullptr));
    message(Out, Id, "run", "error", sth, Arg, Arg2);
}



CmdSthList pm_end_feed("end-feed", "");;
CmdSthList pm_end_feed_missing("end-feed", "missing");
CmdSthList pm_end_feed_not_open("end-feed", "not-open");
CmdSthList pm_run_missing("run", "missing");
CmdSthList pm_feed_missing("feed", "missing");
//ListMessage(Out, Id, "end-feed", "", ended);
//ListMessage(Out, Id, "end-feed", "missing", not_found);
//ListMessage(Out, Id, "end-feed", "not-open", not_open);
//ListMessage(Out, Id, Caller, "missing", missing);

NullNtfSthIdInt pm_process_started("process", "started");
NullNtfSthIdInt pm_process_ended("process", "ended");
//Message(*other, "process", "ended", *id, pid);
//Message(*other, "process", "started", *id, pid);

CmdSthInt pm_run_exit("run", "exit");
CmdSthInt pm_run_signal("run", "signal");
CmdSthInt pm_run_stopped("run", "stopped");
CmdSthInt pm_run_running("run", "running");
//Message(out, *id, "run", "exit", WEXITSTATUS(status));
//Message(out, *id, "run", "signal", WTERMSIG(status));
//Message(out, *id, "run", "stopped", WSTOPSIG(status));
//Message(out, *id, "run", "running", pid);

CmdSthOpt3 pm_run_error_pipe("run", "error", "pipe");
//Message(out, *id, "run", "error", "pipe");
CmdSthOpt3 pm_run_error_duplicate_stdout("run", "error", "duplicate", "stdout");
//Message(out, *id, "run", "duplicate", "stdout");
CmdSthOpt3 pm_run_error_duplicate_stderr("run", "error", "duplicate", "stderr");
//Message(out, *id, "run", "duplicate", "stderr");
CmdSthOpt3 pm_run_error_no_processes("run", "error", "no-processes");
//Message(out, *id, "run", "error", "no-processes");
CmdSthOpt3 pm_run_error_no_memory("run", "error", "no-memory");
//Message(out, *id, "run", "error", "no-memory");
CmdSthOpt3 pm_run_error_input_failed("run", "error", "input", "failed");
//Message(out, *id, "run", "input", "failed"); "error"
CmdSthOpt3 pm_run_input_closed("run", "input", "closed");
//Message(out, *id, "run", "input", "closed");
CmdSthOpt3 pm_run_terminated("run", "terminated");
CmdSthOpt3 pm_run_finished("run", "finished");
//Message(out, *id, "run", terminate ? "terminated" : "finished");
CmdSthOpt3 pm_run_error_no_thread("run", "error", "no-thread");
//Message(out, *id, "run", "error", "no-thread");
CmdSthOpt3 pm_run_error_exception("run", "error", "exception");
//Message(out, *id, "run", "error", "exception");
CmdSthOpt3 pm_run_error_identifier_in_use(
    "run", "error", "identifier", "in-use");
//Message(Out, Id, "run", "error", "identifier", "in-use");
CmdSthOpt3 pm_run_error_channel_in_multiple(
    "run", "error", "channel", "in", "multiple");
//Message(Out, Id, "run", "error", "channel", "in", "multiple");
CmdSthOpt3 pm_run_error_notify_no_input("run", "error", "notify", "no-input");
//Message(Out, Id, "run", "error", "notify", "no-input");
CmdSthOpt3 pm_run_error_channel_in_missing(
    "run", "error", "channel", "in", "missing");
//Message(Out, Id, "run", "error", "channel", "in", "missing");
CmdSthOpt3 pm_run_error_channel_out_missing(
    "run", "error", "channel", "out", "missing");
//Message(Out, Id, "run", "error", "channel", "out", "missing");
CmdSthOpt3 pm_feed_error_not_found("feed", "error", "not-found");
//Message(Out, Id, "feed", "error", "not-found");
CmdSthOpt3 pm_feed_error_closed("feed", "error", "closed");
//Message(Out, Id, "feed", "error", "closed");

CmdErrorArgArgumentSth pm_run_error_cmd_argument_missing("run", "missing");
CmdErrorArgArgumentSth pm_feed_error_cmd_argument_missing("feed", "missing");
//Message(Out, Id, cmd ? cmd : "run", "error", command.c_str(), "argument", "missing");
CmdErrorArgArgumentSth pm_run_error_cmd_argument_not_string(
    "run", "not-string");
CmdErrorArgArgumentSth pm_feed_error_cmd_argument_not_string(
    "feed", "not-string");
//Message(Out, Id, cmd ? cmd : "run", "error", command.c_str(), "argument", "not-string");
CmdErrorArgArgumentSth pm_run_error_cmd_argument_not_string_null(
    "run", "not-string-or-null");
CmdErrorArgArgumentSth pm_feed_error_cmd_argument_not_string_null(
    "feed", "not-string-or-null");
//Message(Out, Id, cmd ? cmd : "run", "error", command.c_str(), "argument", "not-string", "not-null");
CmdErrorArgArgumentSth pm_run_error_cmd_argument_unknown("run", "unknown");
CmdErrorArgArgumentSth pm_feed_error_cmd_argument_unknown("feed", "unknown");
//Message(Out, Id,  Caller, "error", command.c_str(), "argument", "unknown");

CmdErrorArgDuplicateArg pm_run_error_cmd_duplicate("run");
CmdErrorArgDuplicateArg pm_feed_error_cmd_duplicate("feed");
//Message(Out, Id, Caller, "error", command.c_str(), "duplicate", input_values.back()->Name()->String().c_str());

RunErrorSth2OptArg pm_run_error_channel_unknown("channel", "unknown");
//Message(Out, Id, "run", "error", "channel", "unknown", channel.c_str());
RunErrorSth2OptArg pm_run_error_notify_unknown("notify", "unknown");
//Message(Out, Id, "run", "error", "notify", "unknown", Parameters[k + 1]->String().c_str());
RunErrorSth2OptArg pm_run_error_env_invalid("env", "argument", "invalid");
//Message(Out, Id, "run", "error", "env", "argument", "invalid", name.c_str());
RunErrorSth2OptArg pm_run_error_env_duplicate("env", "argument", "duplicate");
//Message(Out, Id, "run", "error", "env", "argument", "duplicate", name.c_str());
RunErrorSth2OptArg pm_run_error_channel_format_unknown(
    "channel", "format", "unknown");
//Message(Out, Id, "run", "error", "channel", "format", "unknown", format.c_str());
RunErrorSth2OptArg pm_run_error_channel_direction_unknown(
    "channel", "direction", "unknown");
//Message(Out, Id, "run", "error", "channel", "direction", "unknown", direction.c_str());
RunErrorSth2OptArg pm_run_error_channel_out_duplicate(
    "channel", "out", "duplicate");
//Message(Out, Id, "run", "error", "channel", "out", "duplicate", nl.first.String().c_str());
RunErrorSth2OptArg pm_run_error_argument_unknown("argument", "unknown");;
//Message(Out, Id, "run", "error", "argument", "unknown", command.c_str());

RunErrorSthArg2 pm_run_error_cd_errmsg("change-directory", 2);
//Message(Out, Id, "run", "error", "change-directory", directory.c_str(), strerror(errno));
RunErrorSthArg2 pm_run_error_program_errmsg("program", 2);
//Message(Out, Id, "run", "error", "program", program_name.c_str(), strerror(errno));
RunErrorSthArg2 pm_run_error_program("program", 1);
//Message(Out, Id, "run", "error", "program", program_name.c_str());
