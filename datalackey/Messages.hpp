//
//  Messages.hpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 21.11.2018
//  Copyright © 2018 Ismo Kärkkäinen. All rights reserved.
//

#ifndef Messages_hpp
#define Messages_hpp


#include "Message.hpp"


class CmdErrorArgumentSth : public Message {
private:
    const char* const cmd;
    const char* const sth;

public:
    CmdErrorArgumentSth(const char* const Cmd, const char* const Sth);

    void Report(Output& Out) const;
    void Send(Output& Out, const SimpleValue& Id) const;
};


class ArgErrorArgumentSth : public Message {
private:
    const char* const sth;

public:
    ArgErrorArgumentSth(const char* const Sth);

    void Report(Output& Out) const;
    void Send(Output& Out, const SimpleValue& Id, const char* const Arg) const;
};


class CmdErrorArgumentSthArg : public Message {
private:
    const char* const cmd;
    const char* const sth;

public:
    CmdErrorArgumentSthArg(const char* const Cmd, const char* const Sth);

    void Report(Output& Out) const;
    void Send(Output& Out, const SimpleValue& Id, const char* const Arg) const;
};


class NullErrorSthOpt : public Message {
private:
    const char* const sth;
    const char* const opt;

public:
    NullErrorSthOpt(const char* const Sth, const char* const Opt = nullptr);

    void Report(Output& Out) const;
    void Send(Output& Out) const;
};


class ErrorCommandSth : public Message {
private:
    const char* const sth;

public:
    ErrorCommandSth(const char* const Sth);

    void Report(Output& Out) const;
    void Send(Output& Out, const SimpleValue& Id) const;
};


class ErrorCommandSthArg : public Message {
private:
    const char* const sth;

public:
    ErrorCommandSthArg(const char* const Sth);

    void Report(Output& Out) const;
    void Send(Output& Out, const SimpleValue& Id, const char* const Arg) const;
};


class CmdSthList : public Message {
private:
    const char* const cmd;
    const char* const sth;

public:
    CmdSthList(const char* const Cmd, const char* const Sth);

    void Report(Output& Out) const;
    void Send(Output& Out, const SimpleValue& Id,
        const std::vector<std::shared_ptr<SimpleValue>>& List) const;
    void Send(Output& Out, const SimpleValue& Id,
        const std::vector<std::string>& List) const;
};


class NullNtfSthArg : public Message {
private:
    const char* const ntf;
    const char* const sth;
    int placeholder_count;

public:
    NullNtfSthArg(
        const char* const Ntf, const char* const Sth, int PlaceholderCount);

    void Report(Output& Out) const;
    void Send(Output& Out,
        const char* const Arg, const char* const Arg2 = nullptr) const;
};


class NullNtfSthList : public Message {
private:
    const char* const ntf;
    const char* const sth;

public:
    NullNtfSthList(const char* const Ntf, const char* const Sth);

    void Report(Output& Out) const;
    void Send(Output& Out,
        const std::vector<std::shared_ptr<SimpleValue>>& List) const;
    void Send(Output& Out, const std::vector<std::string>& List) const;
};


class CmdSthArg2 : public Message {
private:
    const char* const cmd;
    const char* const sth;

public:
    CmdSthArg2(const char* const Cmd, const char* const Sth);

    void Report(Output& Out) const;
    void Send(Output& Out, const SimpleValue& Id,
        const char* const Arg, const char* const Arg2) const;
};


class Sth2Opt : public Message {
private:
    const char* const sth;
    const char* const sth2;
    const char* const opt;

public:
    Sth2Opt(const char* const Sth, const char* const Sth2,
        const char* const Opt = nullptr);

    void Report(Output& Out) const;
    void Send(Output& Out, const SimpleValue& Id) const;
};


class Sth2List : public Message {
private:
    const char* const sth;
    const char* const sth2;

public:
    Sth2List(const char* const Sth, const char* const Sth2);

    void Report(Output& Out) const;
    void Send(Output& Out, const SimpleValue& Id,
        const std::vector<std::string>& List) const;
};


// Message objects for various format or storage-specific classes.
extern NullErrorSthOpt msg_null_error_format;
extern NullErrorSthOpt msg_error_identifier_missing;
extern NullErrorSthOpt msg_error_identifier_invalid;
extern ErrorCommandSth msg_error_command_missing;
extern ErrorCommandSth msg_error_command_not_string;
extern ErrorCommandSthArg msg_error_command_unknown;
extern ArgErrorArgumentSth msg_arg_error_argument_not_integer;
extern ArgErrorArgumentSth msg_arg_error_argument_invalid;

extern NullNtfSthList ntf_data_deleted;
extern NullNtfSthList ntf_data_renamed;
extern NullNtfSthList ntf_data_stored;

extern Sth2Opt msg_channel_reset;
extern Sth2Opt msg_run_error_format;
extern Sth2Opt msg_error_format;

extern Sth2Opt msg_error_identifier_not_string;
extern Sth2List msg_data_stored;


#endif /* Messages_hpp */
