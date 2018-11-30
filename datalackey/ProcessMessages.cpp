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



Sth2OptList pm_end_feed("end-feed", "");;
Sth2OptList pm_end_feed_missing("end-feed", "missing");
Sth2OptList pm_end_feed_not_open("end-feed", "not-open");
Sth2OptList pm_run_error_missing("run", "error", "missing");
Sth2OptList pm_feed_error_missing("feed", "error", "missing");

NullNtfSthIdInt pm_process_started("process", "started");
NullNtfSthIdInt pm_process_ended("process", "ended");

CmdSthInt pm_run_exit("run", "exit");
CmdSthInt pm_run_signal("run", "signal");
CmdSthInt pm_run_stopped("run", "stopped");
CmdSthInt pm_run_running("run", "running");
Sth2Opt3 pm_run_continued("run", "continued");

Sth2Opt3 pm_run_error_pipe("run", "error", "pipe");
Sth2Opt3 pm_run_error_duplicate_stdout("run", "error", "duplicate", "stdout");
Sth2Opt3 pm_run_error_duplicate_stderr("run", "error", "duplicate", "stderr");
Sth2Opt3 pm_run_error_no_processes("run", "error", "no-processes");
Sth2Opt3 pm_run_error_no_memory("run", "error", "no-memory");
Sth2Opt3 pm_run_error_input_failed("run", "error", "input", "failed");
Sth2Opt3 pm_run_input_closed("run", "input", "closed");
Sth2Opt3 pm_run_terminated("run", "terminated");
Sth2Opt3 pm_run_finished("run", "finished");
Sth2Opt3 pm_run_error_no_thread("run", "error", "no-thread");
Sth2Opt3 pm_run_error_exception("run", "error", "exception");
Sth2Opt3 pm_run_error_identifier_in_use("run", "error", "identifier", "in-use");
Sth2Opt3 pm_run_error_in_multiple("run", "error", "in", "multiple");
Sth2Opt3 pm_run_error_notify_no_input("run", "error", "notify", "no-input");
Sth2Opt3 pm_run_error_in_missing("run", "error", "in", "missing");
Sth2Opt3 pm_run_error_out_missing("run", "error", "out", "missing");
Sth2Opt3 pm_feed_error_not_found("feed", "error", "not-found");
Sth2Opt3 pm_feed_error_closed("feed", "error", "closed");

CmdErrorArgArgumentSth pm_run_error_cmd_argument_missing("run", "missing");
CmdErrorArgArgumentSth pm_feed_error_cmd_argument_missing("feed", "missing");
CmdErrorArgArgumentSth pm_run_error_cmd_argument_not_string(
    "run", "not-string");
CmdErrorArgArgumentSth pm_feed_error_cmd_argument_not_string(
    "feed", "not-string");
CmdErrorArgArgumentSth pm_run_error_cmd_argument_not_string_null(
    "run", "not-string-or-null");
CmdErrorArgArgumentSth pm_run_error_cmd_argument_unknown("run", "unknown");
CmdErrorArgArgumentSth pm_feed_error_cmd_argument_unknown("feed", "unknown");

CmdErrorArgDuplicateArg pm_run_error_cmd_duplicate("run");
CmdErrorArgDuplicateArg pm_feed_error_cmd_duplicate("feed");

RunErrorSth2OptArg pm_run_error_in_unknown("in", "unknown");
RunErrorSth2OptArg pm_run_error_out_unknown("out", "unknown");
RunErrorSth2OptArg pm_run_error_notify_unknown("notify", "unknown");
RunErrorSth2OptArg pm_run_error_env_invalid("env", "argument", "invalid");
RunErrorSth2OptArg pm_run_error_env_duplicate("env", "argument", "duplicate");
RunErrorSth2OptArg pm_run_error_in_format_unknown(
    "in", "format", "unknown");
RunErrorSth2OptArg pm_run_error_out_format_unknown(
    "out", "format", "unknown");
RunErrorSth2OptArg pm_run_error_out_duplicate(
    "out", "duplicate");
RunErrorSth2OptArg pm_run_error_argument_unknown("argument", "unknown");;

RunErrorSthArg2 pm_run_error_cd_errmsg("change-directory", 2);
RunErrorSthArg2 pm_run_error_program_errmsg("program", 2);
RunErrorSthArg2 pm_run_error_program("program", 1);
