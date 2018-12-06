//
//  Message.hpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 21.11.2018
//  Copyright © 2018 Ismo Kärkkäinen. All rights reserved.
//

#include "Message.hpp"
#include "MessageReporter.hpp"
#include "Output.hpp"
#include "Value_t.hpp"
#include "Number_t.hpp"
#include "Structure.hpp"
#include "StringValue.hpp"
#include "NumberValue.hpp"
#include "NullValue.hpp"
#include <cassert>
#include <memory>


StringValue Message::id("id");
const char* const Message::item = "?";
std::shared_ptr<SimpleValue> Message::dots(new StringValue("..."));

Message::Message() {
    MessageReporter::Get().Register(this);
}

Message::~Message() { }

void Message::Feed(OutputItem& Writer, const SimpleValue& Id) {
    if (IsStringValue(&Id)) {
        Writer << ValueRef<std::string>(Id.String());
        return;
    }
    if (IsNumberValue(&Id)) {
        Writer << NumberRef<long long int>(Id.Number());
        return;
    }
    if (IsNullValue(&Id)) {
        Writer << Null;
        return;
    }
    assert(false);
}


void Message::msg(Output& Out, const SimpleValue* Id,
    const char *const Class, const char *const Kind, const char *const three,
    const char *const four, const char *const five, const char *const six,
    int* last, long long int* big_last, bool null_last, int* real_last) const
{
    std::unique_ptr<OutputItem> writer(
        Out.Writable(Id != nullptr && IsNullValue(Id)));
    *writer << Array;
    if (Id == nullptr)
        *writer << Structure::Null;
    else
        feed(*writer, *Id);
    *writer << ValueRef<std::string>(Class)
        << ValueRef<std::string>(Kind);
    if (three != nullptr)
        *writer << ValueRef<std::string>(three);
    if (four != nullptr)
        *writer << ValueRef<std::string>(four);
    if (five != nullptr)
        *writer << ValueRef<std::string>(five);
    if (six != nullptr)
        *writer << ValueRef<std::string>(six);
    if (last != nullptr)
        *writer << NumberRef<int>(*last);
    if (big_last != nullptr)
        *writer << NumberRef<long long int>(*big_last);
    if (null_last)
        *writer << Structure::Null;
    if (real_last != nullptr)
        *writer << NumberRef<int>(*real_last);
    *writer << End;
}

void Message::message(Output& Out,
    const char *const Class, const char *const Kind,
    const char *const three, const char *const four,
    const char *const five, const char *const six) const
{
    msg(Out, nullptr, Class, Kind, three, four, five, six, nullptr);
}

void Message::message(Output& Out, const SimpleValue* Id,
    const char *const Class, const char *const Kind,
    const char *const three, const char *const four,
    const char *const five, const char *const six) const
{
    msg(Out, Id, Class, Kind, three, four, five, six, nullptr);
}

void Message::message(Output& Out, const SimpleValue& Id,
    const char *const Class, const char *const Kind,
    const char *const three, const char *const four,
    const char *const five, const char *const six) const
{
    msg(Out, &Id, Class, Kind, three, four, five, six, nullptr);
}

void Message::message(Output& Out, const SimpleValue& Id,
    const char *const Class, const char *const Kind, int last,
    const char *const three, const char *const four,
    const char *const five, const char *const six) const
{
    msg(Out, &Id, Class, Kind, three, four, five, six, &last);
}

void Message::message(Output& Out,
    const char *const Class, const char *const Kind,
    const SimpleValue& second_to_last, int last,
    const char *const three, const char *const four, const char *const five)
    const
{
    if (IsStringValue(&second_to_last)) {
        msg(Out, nullptr, Class, Kind, three, four, five,
            second_to_last.String().c_str(), nullptr, nullptr, false, &last);
    } else if (IsNumberValue(&second_to_last)) {
        long long int val = second_to_last.Number();
        msg(Out, nullptr, Class, Kind, three, four, five,
            nullptr, nullptr, &val, false, &last);
    } else if (IsNullValue(&second_to_last)) {
        msg(Out, nullptr, Class, Kind, three, four, five,
            nullptr, nullptr, nullptr, true, &last);
    }
}

void Message::feed(OutputItem& Writer, const SimpleValue& Id) const {
    Message::Feed(Writer, Id);
}

void Message::listmsg(Output& Out, const SimpleValue* Id,
    const char *const Class, const char *const Kind,
    const std::vector<std::shared_ptr<SimpleValue>>& List,
    const char *const OptPreList, const char *const OptPreList2) const
{
    std::unique_ptr<OutputItem> writer(
        Out.Writable(Id != nullptr && IsNullValue(Id)));
    *writer << Array;
    if (Id == nullptr)
        *writer << Structure::Null;
    else
        feed(*writer, *Id);
    *writer << ValueRef<std::string>(Class)
        << ValueRef<std::string>(Kind);
    if (OptPreList != nullptr)
        *writer << ValueRef<std::string>(OptPreList);
    if (OptPreList2 != nullptr)
        *writer << ValueRef<std::string>(OptPreList2);
    for (auto arg : List)
        feed(*writer, *arg);
    *writer << End;
}

void Message::listmsg(Output& Out, const SimpleValue* Id,
    const char *const Class, const char *const Kind,
    const std::vector<std::string>& List, const char *const OptPreList,
    const char *const OptPreList2) const
{
    std::unique_ptr<OutputItem> writer(
        Out.Writable(Id != nullptr && IsNullValue(Id)));
    *writer << Array;
    if (Id == nullptr)
        *writer << Structure::Null;
    else
        feed(*writer, *Id);
    *writer << ValueRef<std::string>(Class)
        << ValueRef<std::string>(Kind);
    if (OptPreList != nullptr)
        *writer << ValueRef<std::string>(OptPreList);
    if (OptPreList2 != nullptr)
        *writer << ValueRef<std::string>(OptPreList2);
    for (auto arg : List)
        *writer << ValueRef<std::string>(arg);
    *writer << End;
}

void Message::listmessage(Output& Out, const SimpleValue& Id,
    const char *const Class, const char *const Kind,
    const std::vector<std::shared_ptr<SimpleValue>>& List,
    const char *const OptPreList, const char *const OptPreList2) const
{
    listmsg(Out, &Id, Class, Kind, List, OptPreList, OptPreList2);
}

void Message::listmessage(Output& Out, const SimpleValue& Id,
    const char *const Class, const char *const Kind,
    const std::vector<std::string>& List, const char *const OptPreList,
    const char *const OptPreList2) const
{
    listmsg(Out, &Id, Class, Kind, List, OptPreList, OptPreList2);
}

void Message::listmessage(Output& Out,
    const char *const Class, const char *const Kind,
    const std::vector<std::shared_ptr<SimpleValue>>& List,
    const char *const OptPreList, const char *const OptPreList2) const
{
    listmsg(Out, nullptr, Class, Kind, List, OptPreList, OptPreList2);
}

void Message::listmessage(Output& Out,
    const char *const Class, const char *const Kind,
    const std::vector<std::string>& List, const char *const OptPreList,
    const char *const OptPreList2) const
{
    listmsg(Out, nullptr, Class, Kind, List, OptPreList, OptPreList2);
}
