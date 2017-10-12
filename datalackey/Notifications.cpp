//
//  Notifications.cpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 4.10.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#include "Notifications.hpp"
#include "Value_t.hpp"


static void message(Output& Out, const char *const type, const char *const one,
    const char *const two, const char *const three, const char *const four)
{
    OutputItem* writer = Out.Writable();
    *writer << Array
        << ValueRef<std::string>(type)
        << ValueRef<std::string>(one);
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
    message(Out, "note", one, two, three, four);
}

void Error(Output& Out, const char *const one, const char *const two,
    const char *const three, const char *const four)
{
    message(Out, "error", one, two, three, four);
}