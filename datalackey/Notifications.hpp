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


void Note(Output& Out, const char *const one, const char *const two = nullptr,
    const char *const three = nullptr, const char *const four = nullptr);
void Note(Output& Out, const SimpleValue& Id, const char *const one,
    const char *const two = nullptr, const char *const three = nullptr,
    const char *const four = nullptr);

void Error(Output& Out, const char *const one, const char *const two = nullptr,
    const char *const three = nullptr, const char *const four = nullptr);
void Error(Output& Out, const SimpleValue& Id, const char *const one,
    const char *const two = nullptr, const char *const three = nullptr,
    const char *const four = nullptr);

void Feed(OutputItem& Writer, const SimpleValue& Id);


#endif /* Notifications_hpp */
