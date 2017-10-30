//
//  Notifications.cpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 4.10.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#include "Notifications.hpp"
#include "Value_t.hpp"


static void message(Output& Out, const SimpleValue* Id, const char *const type,
    const char *const one, const char *const two, const char *const three,
    const char *const four)
{
    OutputItem* writer = Out.Writable();
    if (Id == nullptr)
        *writer << Structure::Null;
    else
        Feed(*writer, *Id);
    *writer << Array
        << ValueRef<std::string>(type);
    if (one != nullptr)
        *writer << ValueRef<std::string>(one);
    if (two != nullptr)
        *writer << ValueRef<std::string>(two);
    if (three != nullptr)
        *writer << ValueRef<std::string>(three);
    if (four != nullptr)
        *writer << ValueRef<std::string>(four);
    *writer << End;
    delete writer;
}

void Note(Output& Out, const char *const one, const char *const two,
    const char *const three, const char *const four)
{
    message(Out, nullptr, "note", one, two, three, four);
}

void Note(Output& Out, const SimpleValue& Id, const char *const one,
    const char *const two, const char *const three, const char *const four)
{
    message(Out, &Id, "note", one, two, three, four);
}

void Error(Output& Out, const char *const one, const char *const two,
    const char *const three, const char *const four)
{
    message(Out, nullptr, "error", one, two, three, four);
}

void Error(Output& Out, const SimpleValue& Id, const char *const one,
    const char *const two, const char *const three, const char *const four)
{
    message(Out, &Id, "error", one, two, three, four);
}

void Feed(OutputItem& Writer, const SimpleValue& Id) {
    if (Id.IsNumber())
        Writer << ValueRef<long long int>(Id.Number());
    else
        Writer << ValueRef<std::string>(Id.String());
}
