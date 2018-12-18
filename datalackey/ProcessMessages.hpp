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


extern Sth2Opt2List pm_end_feed_missing;
extern Sth2Opt2List pm_end_feed_not_open;
extern Sth2Opt2List pm_run_error_missing;
extern Sth2Opt2List pm_feed_error_missing;

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
extern Sth2Opt3 pm_run_continued;

extern Sth2Opt3 pm_run_error_pipe;
extern Sth2Opt3 pm_run_error_no_processes;
extern Sth2Opt3 pm_run_error_no_memory;
extern Sth2Opt3 pm_run_error_input_failed;
extern Sth2Opt3 pm_run_input_closed;
extern Sth2Opt3 pm_run_terminated;
extern Sth2Opt3 pm_run_finished;
extern Sth2Opt3 pm_run_error_no_thread;
extern Sth2Opt3 pm_run_error_exception;
extern Sth2Opt3 pm_run_error_identifier_in_use;
extern Sth2Opt3 pm_run_error_in_multiple;
extern Sth2Opt3 pm_run_error_notify_no_input;
extern Sth2Opt3 pm_run_error_in_missing;
extern Sth2Opt3 pm_run_error_out_missing;
extern Sth2Opt3 pm_feed_error_not_found;
extern Sth2Opt3 pm_feed_error_closed;

class CmdErrorArgArgumentSth : public Message {
private:
    const char* const cmd;
    const char* const sth;

public:
    CmdErrorArgArgumentSth(const char* const Cmd, const char* const Sth);

    void Report(Output& Out) const;
    void Send(Output& Out, const SimpleValue& Id, const char* const Arg) const;
};

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


extern RunErrorSth2OptArg pm_run_error_env_invalid;
extern RunErrorSth2OptArg pm_run_error_env_duplicate;
extern RunErrorSth2OptArg pm_run_error_out_duplicate;
extern RunErrorSth2OptArg pm_run_error_output_duplicate;

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
