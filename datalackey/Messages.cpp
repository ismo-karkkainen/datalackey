//
//  Messages.cpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 4.10.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#include "Messages.hpp"
#include "Value_t.hpp"
#include "Number_t.hpp"
#include "Structure.hpp"
#include "StringValue.hpp"
#include "NumberValue.hpp"
#include "NullValue.hpp"
#include <cassert>
#include <memory>


static void message(Output& Out, const SimpleValue* Id,
    const char *const Class, const char *const Kind, const char *const three,
    const char *const four, const char *const five, const char *const six,
    int* last, long long int* big_last = nullptr, bool null_last = false,
    int* real_last = nullptr)
{
    std::unique_ptr<OutputItem> writer(
        Out.Writable(Id != nullptr && IsNullValue(Id)));
    *writer << Array;
    if (Id == nullptr)
        *writer << Structure::Null;
    else
        Feed(*writer, *Id);
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

void Message(Output& Out, const char *const Class, const char *const Kind,
    const char *const three, const char *const four,
    const char *const five, const char *const six)
{
    message(Out, nullptr, Class, Kind, three, four, five, six, nullptr);
}

void Message(Output& Out, const SimpleValue* Id, const char *const Class,
    const char *const Kind, const char *const three, const char *const four,
    const char *const five, const char *const six)
{
    message(Out, Id, Class, Kind, three, four, five, six, nullptr);
}

void Message(Output& Out, const SimpleValue& Id, const char *const Class,
    const char *const Kind, const char *const three, const char *const four,
    const char *const five, const char *const six)
{
    message(Out, &Id, Class, Kind, three, four, five, six, nullptr);
}

void Message(Output& Out, const SimpleValue& Id,
    const char *const Class, const char *const Kind, int last,
    const char *const three, const char *const four,
    const char *const five, const char *const six)
{
    message(Out, &Id, Class, Kind, three, four, five, six, &last);
}

void Message(Output& Out, const char *const Class, const char *const Kind,
    const SimpleValue& second_to_last, int last,
    const char *const three, const char *const four, const char *const five)
{
    if (IsStringValue(&second_to_last)) {
        message(Out, nullptr, Class, Kind, three, four, five,
            second_to_last.String().c_str(), nullptr, nullptr, false, &last);
    } else if (IsNumberValue(&second_to_last)) {
        long long int val = second_to_last.Number();
        message(Out, nullptr, Class, Kind, three, four, five,
            nullptr, nullptr, &val, false, &last);
    } else if (IsNullValue(&second_to_last)) {
        message(Out, nullptr, Class, Kind, three, four, five,
            nullptr, nullptr, nullptr, true, &last);
    }
}

void Feed(OutputItem& Writer, const SimpleValue& Id) {
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

static void ListMessage(Output& Out, const SimpleValue* Id,
    const char *const Class, const char *const Kind,
    std::vector<std::shared_ptr<SimpleValue>>& List)
{
    std::unique_ptr<OutputItem> writer(
        Out.Writable(Id != nullptr && IsNullValue(Id)));
    *writer << Array;
    if (Id == nullptr)
        *writer << Structure::Null;
    else
        Feed(*writer, *Id);
    *writer << ValueRef<std::string>(Class)
        << ValueRef<std::string>(Kind);
    for (auto arg : List)
        Feed(*writer, *arg);
    *writer << End;
}

static void ListMessage(Output& Out, const SimpleValue* Id,
    const char *const Class, const char *const Kind,
    std::vector<std::string>& List)
{
    std::unique_ptr<OutputItem> writer(
        Out.Writable(Id != nullptr && IsNullValue(Id)));
    *writer << Array;
    if (Id == nullptr)
        *writer << Structure::Null;
    else
        Feed(*writer, *Id);
    *writer << ValueRef<std::string>(Class)
        << ValueRef<std::string>(Kind);
    for (auto arg : List)
        *writer << ValueRef<std::string>(arg);
    *writer << End;
}

void ListMessage(Output& Out, const SimpleValue& Id,
    const char *const Class, const char *const Kind,
    std::vector<std::shared_ptr<SimpleValue>>& List)
{
    ListMessage(Out, &Id, Class, Kind, List);
}

void ListMessage(Output& Out, const SimpleValue& Id,
    const char *const Class, const char *const Kind,
    std::vector<std::string>& List)
{
    ListMessage(Out, &Id, Class, Kind, List);
}

void ListMessage(Output& Out, const char *const Class, const char *const Kind,
    std::vector<std::shared_ptr<SimpleValue>>& List)
{
    ListMessage(Out, nullptr, Class, Kind, List);
}

void ListMessage(Output& Out, const char *const Class, const char *const Kind,
    std::vector<std::string>& List)
{
    ListMessage(Out, nullptr, Class, Kind, List);
}
