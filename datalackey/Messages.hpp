//
//  Messages.hpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 4.10.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#ifndef Messages_hpp
#define Messages_hpp

#include "Output.hpp"
#include "SimpleValue.hpp"
#include <memory>


// If Id type is NullValue there is no output, except for Feed.

void Message(Output& Out, const char *const Class, const char *const Kind,
    const char *const three = nullptr, const char *const four = nullptr,
    const char *const five = nullptr, const char *const six = nullptr);
void Message(Output& Out, const SimpleValue* Id,
    const char *const Class, const char *const Kind,
    const char *const three = nullptr, const char *const four = nullptr,
    const char *const five = nullptr, const char *const six = nullptr);
void Message(Output& Out, const SimpleValue& Id,
    const char *const Class, const char *const Kind,
    const char *const three = nullptr, const char *const four = nullptr,
    const char *const five = nullptr, const char *const six = nullptr);
void Message(Output& Out, const SimpleValue& Id,
    const char *const Class, const char *const Kind, int last,
    const char *const three = nullptr, const char *const four = nullptr,
    const char *const five = nullptr, const char *const six = nullptr);
void Message(Output& Out,
    const char *const Class, const char *const Kind, const SimpleValue& last,
    const char *const three = nullptr, const char *const four = nullptr,
    const char *const five = nullptr);

void Feed(OutputItem& Writer, const SimpleValue& Id);

void ListMessage(Output& Out, const SimpleValue& Id, const char *const Class,
    const char *const Kind, std::vector<std::shared_ptr<SimpleValue>>& List);
void ListMessage(Output& Out, const SimpleValue& Id, const char *const Class,
    const char *const Kind, std::vector<std::string>& List);

void ListMessage(Output& Out, const char *const Class, const char *const Kind,
    std::vector<std::shared_ptr<SimpleValue>>& List);
void ListMessage(Output& Out, const char *const Class, const char *const Kind,
    std::vector<std::string>& List);


#endif /* Messages_hpp */
