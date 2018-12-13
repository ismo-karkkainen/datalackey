//
//  Message.hpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 21.11.2018
//  Copyright © 2018 Ismo Kärkkäinen. All rights reserved.
//

#ifndef Message_hpp
#define Message_hpp


#include "SimpleValue.hpp"
#include "StringValue.hpp"
#include <memory>
#include <string>
#include <tuple>
#include <vector>


class Output;
class OutputItem;

// Base class to derive actual message classes from.
// Any instance of derived classes is expected to live long enough for
// MessageReporter::Report() to be called in main program, if it is called.
class Message {
protected:
    // For use in derived classes.
    static StringValue id;
    static const char* const item;
    static std::shared_ptr<SimpleValue> dots;

    void msg(Output& Out, const SimpleValue* Id,
        const char *const Class, const char *const Kind,
        const char *const three, const char *const four,
        const char *const five, const char *const six,
        int* last, long long int* big_last = nullptr, bool null_last = false,
        int* real_last = nullptr) const;

    // If Id type is NullValue there is no output, except for Feed.

    void message(Output& Out, const char *const Class, const char *const Kind,
        const char *const three = nullptr, const char *const four = nullptr,
        const char *const five = nullptr, const char *const six = nullptr)
        const;
    void message(Output& Out, const SimpleValue* Id,
        const char *const Class, const char *const Kind,
        const char *const three = nullptr, const char *const four = nullptr,
        const char *const five = nullptr, const char *const six = nullptr)
        const;
    void message(Output& Out, const SimpleValue& Id,
        const char *const Class, const char *const Kind,
        const char *const three = nullptr, const char *const four = nullptr,
        const char *const five = nullptr, const char *const six = nullptr)
        const;
    void message(Output& Out, const SimpleValue& Id,
        const char *const Class, const char *const Kind, int last,
        const char *const three = nullptr, const char *const four = nullptr,
        const char *const five = nullptr, const char *const six = nullptr)
        const;
    void message(Output& Out,
        const char *const Class, const char *const Kind,
        const SimpleValue& second_to_last, int last,
        const char *const three = nullptr, const char *const four = nullptr,
        const char *const five = nullptr) const;

    void feed(OutputItem& Writer, const SimpleValue& Id) const;

    void listmsg(Output& Out, const SimpleValue* Id,
        const char *const Class, const char *const Kind,
        const std::vector<std::shared_ptr<SimpleValue>>& List,
        const char *const OptPreList = nullptr,
        const char *const OptPreList2 = nullptr) const;
    void listmsg(Output& Out, const SimpleValue* Id,
        const char *const Class, const char *const Kind,
        const std::vector<std::string>& List,
        const char *const OptPreList = nullptr,
        const char *const OptPreList2 = nullptr) const;
    void listmessage(Output& Out, const SimpleValue& Id,
        const char *const Class, const char *const Kind,
        const std::vector<std::shared_ptr<SimpleValue>>& List,
        const char *const OptPreList = nullptr,
        const char *const OptPreList2 = nullptr) const;
    void listmessage(Output& Out, const SimpleValue& Id,
        const char *const Class, const char *const Kind,
        const std::vector<std::string>& List,
        const char *const OptPreList = nullptr,
        const char *const OptPreList2 = nullptr) const;

    void listmessage(Output& Out,
        const char *const Class, const char *const Kind,
        const std::vector<std::shared_ptr<SimpleValue>>& List,
        const char *const OptPreList = nullptr,
        const char *const OptPreList2 = nullptr) const;
    void listmessage(Output& Out,
        const char *const Class, const char *const Kind,
        const std::vector<std::string>& List,
        const char *const OptPreList = nullptr,
        const char *const OptPreList2 = nullptr) const;

    void mapmessage(Output& Out, const SimpleValue* Id,
        const char *const Class, const char *const Kind,
        const std::vector<std::tuple<std::string, long long int>>& Map,
        bool ConvertZeroToNull = false)
        const;

public:
    Message();
    virtual ~Message();
    virtual void Report(Output& Out) const = 0;

    static void Feed(OutputItem& Writer, const SimpleValue& Id);
};


#endif /* Message_hpp */
