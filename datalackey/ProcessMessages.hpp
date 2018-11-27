//
//  ProcessMessages.hpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 23.11.2018
//  Copyright © 2018 Ismo Kärkkäinen. All rights reserved.
//

#ifndef ProcessMessages_hpp
#define ProcessMessages_hpp


#include "Message.hpp"
#include "Messages.hpp"


extern CmdSthList pm_end_feed;
extern CmdSthList pm_end_feed_missing;
extern CmdSthList pm_end_feed_not_open;
extern CmdSthList pm_run_missing;
extern CmdSthList pm_feed_missing;

class NullNtfSthIdInt : public Message {
private:
    const char* const ntf;
    const char* const sth;

public:
    NullNtfSthIdInt(const char* const Ntf, const char* const Sth);

    void Report(Output& Out) const;
    void Send(Output& Out, const SimpleValue& Id, int Int) const;
};

extern NullNtfSthIdInt pm_process_started;
extern NullNtfSthIdInt pm_process_ended;


class CmdSthInt : public Message {
private:
    const char* const cmd;
    const char* const sth;

public:
    CmdSthInt(const char* const Cmd, const char* const Sth);

    void Report(Output& Out) const;
    void Send(Output& Out, const SimpleValue& Id, int Int) const;
};

extern CmdSthInt pm_run_exit;
extern CmdSthInt pm_run_signal;
extern CmdSthInt pm_run_stopped;
extern CmdSthInt pm_run_running;


class CmdSthOpt3 : public Message {
private:
    const char* const cmd;
    const char* const sth;
    const char* const opt;
    const char* const opt2;
    const char* const opt3;

public:
    CmdSthOpt3(const char* const Cmd, const char* const Sth,
        const char* const Opt = nullptr,
        const char* const Opt2 = nullptr,
        const char* const Opt3 = nullptr);

    void Report(Output& Out) const;
    void Send(Output& Out, const SimpleValue& Id) const;
};

extern CmdSthOpt3 pm_run_error_pipe;
extern CmdSthOpt3 pm_run_error_duplicate_stdout;
extern CmdSthOpt3 pm_run_error_duplicate_stderr;
extern CmdSthOpt3 pm_run_error_no_processes;
extern CmdSthOpt3 pm_run_error_no_memory;
extern CmdSthOpt3 pm_run_error_input_failed;
extern CmdSthOpt3 pm_run_input_closed;
extern CmdSthOpt3 pm_run_terminated;
extern CmdSthOpt3 pm_run_finished;
extern CmdSthOpt3 pm_run_error_no_thread;
extern CmdSthOpt3 pm_run_error_exception;
extern CmdSthOpt3 pm_run_error_identifier_in_use;
extern CmdSthOpt3 pm_run_error_channel_in_multiple;
extern CmdSthOpt3 pm_run_error_notify_no_input;
extern CmdSthOpt3 pm_run_error_channel_in_missing;
extern CmdSthOpt3 pm_run_error_channel_out_missing;
extern CmdSthOpt3 pm_feed_error_not_found;
extern CmdSthOpt3 pm_feed_error_closed;


class CmdErrorArgArgumentSth : public Message {
private:
    const char* const cmd;
    const char* const sth;

public:
    CmdErrorArgArgumentSth(const char* const Cmd, const char* const Sth);

    void Report(Output& Out) const;
    void Send(Output& Out, const SimpleValue& Id, const char* const Arg) const;
};

extern CmdErrorArgArgumentSth pm_run_error_cmd_argument_missing;
extern CmdErrorArgArgumentSth pm_feed_error_cmd_argument_missing;
extern CmdErrorArgArgumentSth pm_run_error_cmd_argument_not_string;
extern CmdErrorArgArgumentSth pm_feed_error_cmd_argument_not_string;
extern CmdErrorArgArgumentSth pm_run_error_cmd_argument_not_string_null;
extern CmdErrorArgArgumentSth pm_feed_error_cmd_argument_not_string_null;
extern CmdErrorArgArgumentSth pm_run_error_cmd_argument_unknown;
extern CmdErrorArgArgumentSth pm_feed_error_cmd_argument_unknown;


class CmdErrorArgDuplicateArg : public Message {
private:
    const char* const cmd;

public:
    CmdErrorArgDuplicateArg(const char* const Cmd);

    void Report(Output& Out) const;
    void Send(Output& Out, const SimpleValue& Id,
        const char* const Arg, const char* const Arg2) const;
};

extern CmdErrorArgDuplicateArg pm_run_error_cmd_duplicate;
extern CmdErrorArgDuplicateArg pm_feed_error_cmd_duplicate;

class RunErrorSth2OptArg : public Message {
private:
    const char* const sth;
    const char* const sth2;
    const char* const opt;

public:
    RunErrorSth2OptArg(const char* const Sth, const char* const Sth2,
        const char* const Opt = nullptr);

    void Report(Output& Out) const;
    void Send(Output& Out, const SimpleValue& Id, const char* const Arg) const;
};


extern RunErrorSth2OptArg pm_run_error_channel_unknown;
extern RunErrorSth2OptArg pm_run_error_notify_unknown;
extern RunErrorSth2OptArg pm_run_error_env_invalid;
extern RunErrorSth2OptArg pm_run_error_env_duplicate;
extern RunErrorSth2OptArg pm_run_error_channel_format_unknown;
extern RunErrorSth2OptArg pm_run_error_channel_direction_unknown;
extern RunErrorSth2OptArg pm_run_error_channel_out_duplicate;
extern RunErrorSth2OptArg pm_run_error_argument_unknown;

class RunErrorSthArg2 : public Message {
private:
    const char* const sth;
    int arg_count;

public:
    RunErrorSthArg2(const char* const Sth, int ArgCount);

    void Report(Output& Out) const;
    void Send(Output& Out, const SimpleValue& Id,
        const char* const Arg, const char* const Arg2 = nullptr) const;
};

extern RunErrorSthArg2 pm_run_error_cd_errmsg;
extern RunErrorSthArg2 pm_run_error_program_errmsg;
extern RunErrorSthArg2 pm_run_error_program;


#endif /* ProcessMessages_hpp */
