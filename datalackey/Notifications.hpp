//
//  Notifications.hpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 4.10.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#ifndef Notifications_hpp
#define Notifications_hpp

#include "Output.hpp"
#include "SimpleValue.hpp"


// If Id type is NullValue there is no output, except for Feed.

void Note(Output& Out, const char *const one, const char *const two = nullptr,
    const char *const three = nullptr, const char *const four = nullptr);
void Note(Output& Out, const SimpleValue* Id, const char *const one,
    const char *const two = nullptr, const char *const three = nullptr,
    const char *const four = nullptr);
void Note(Output& Out, const SimpleValue& Id, const char *const one,
    const char *const two = nullptr, const char *const three = nullptr,
    const char *const four = nullptr);

void Error(Output& Out, const char *const one, const char *const two = nullptr,
    const char *const three = nullptr, const char *const four = nullptr);
void Error(Output& Out, const SimpleValue* Id, const char *const one,
    const char *const two = nullptr, const char *const three = nullptr,
    const char *const four = nullptr);
void Error(Output& Out, const SimpleValue& Id, const char *const one,
    const char *const two = nullptr, const char *const three = nullptr,
    const char *const four = nullptr);

void Message(Output& Out, const char *const one,
    const char *const two = nullptr, const char *const three = nullptr,
    const char *const four = nullptr);
void Message(Output& Out, const SimpleValue& Id,
    const char *const one = nullptr, const char *const two = nullptr,
    const char *const three = nullptr, const char *const four = nullptr);
void Message(Output& Out, const SimpleValue& Id, int last,
    const char *const one = nullptr, const char *const two = nullptr,
    const char *const three = nullptr, const char *const four = nullptr);

void Feed(OutputItem& Writer, const SimpleValue& Id);

void ListMessage(Output& Out, const SimpleValue& Id, const char *const one,
    std::vector<SimpleValue*>& List);


#endif /* Notifications_hpp */
