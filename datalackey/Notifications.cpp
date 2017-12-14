//
//  Notifications.cpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 4.10.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#include "Notifications.hpp"
#include "Value_t.hpp"
#include "Number_t.hpp"
#include "Structure.hpp"
#include "StringValue.hpp"
#include "NumberValue.hpp"
#include "NullValue.hpp"
#include <cassert>
#include <memory>


static void message(Output& Out, const SimpleValue* Id, const char *const type,
    const char *const one, const char *const two, const char *const three,
    const char *const four, int* last)
{
    std::unique_ptr<OutputItem> writer(
        Out.Writable(Id != nullptr && IsNullValue(Id)));
    *writer << Array;
    if (Id == nullptr)
        *writer << Structure::Null;
    else
        Feed(*writer, *Id);
    if (type != nullptr)
        *writer << ValueRef<std::string>(type);
    if (one != nullptr)
        *writer << ValueRef<std::string>(one);
    if (two != nullptr)
        *writer << ValueRef<std::string>(two);
    if (three != nullptr)
        *writer << ValueRef<std::string>(three);
    if (four != nullptr)
        *writer << ValueRef<std::string>(four);
    if (last != nullptr)
        *writer << NumberRef<int>(*last);
    *writer << End;
}

void Note(Output& Out, const char *const one, const char *const two,
    const char *const three, const char *const four)
{
    message(Out, nullptr, "note", one, two, three, four, nullptr);
}

void Note(Output& Out, const SimpleValue* Id, const char *const one,
    const char *const two, const char *const three, const char *const four)
{
    message(Out, Id, "note", one, two, three, four, nullptr);
}

void Note(Output& Out, const SimpleValue& Id, const char *const one,
    const char *const two, const char *const three, const char *const four)
{
    message(Out, &Id, "note", one, two, three, four, nullptr);
}

void Error(Output& Out, const char *const one, const char *const two,
    const char *const three, const char *const four)
{
    message(Out, nullptr, "error", one, two, three, four, nullptr);
}

void Error(Output& Out, const SimpleValue* Id, const char *const one,
    const char *const two, const char *const three, const char *const four)
{
    message(Out, Id, "error", one, two, three, four, nullptr);
}

void Error(Output& Out, const SimpleValue& Id, const char *const one,
    const char *const two, const char *const three, const char *const four)
{
    message(Out, &Id, "error", one, two, three, four, nullptr);
}

void Message(Output& Out, const char *const one, const char *const two,
    const char *const three, const char *const four)
{
    message(Out, nullptr, nullptr, one, two, three, four, nullptr);
}

void Message(Output& Out, const SimpleValue* Id, const char *const one,
    const char *const two, const char *const three, const char *const four)
{
    message(Out, Id, nullptr, one, two, three, four, nullptr);
}

void Message(Output& Out, const SimpleValue& Id, const char *const one,
    const char *const two, const char *const three, const char *const four)
{
    message(Out, &Id, nullptr, one, two, three, four, nullptr);
}

void Message(Output& Out, const SimpleValue& Id, int last,
    const char *const one, const char *const two,
    const char *const three, const char *const four)
{
    message(Out, &Id, nullptr, one, two, three, four, &last);
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

void ListMessage(Output& Out, const SimpleValue& Id, const char *const one,
    std::vector<SimpleValue*>& List)
{
    std::unique_ptr<OutputItem> writer(Out.Writable(IsNullValue(&Id)));
    *writer << Array;
    Feed(*writer, Id);
    *writer << ValueRef<std::string>(one);
    for (auto arg : List)
        Feed(*writer, *arg);
    *writer << End;
}
