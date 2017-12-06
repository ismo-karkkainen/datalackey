//
//  StringValueMapperSimple.cpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 23.10.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#include "StringValueMapperSimple.hpp"


StringValueMapperSimple::StringValueMapperSimple(
    const std::string& Prefix, const std::string& Postfix)
    : prefix(Prefix), postfix(Postfix)
{ }

StringValueMapperSimple::~StringValueMapperSimple() {
    for (auto iter : map)
        delete iter.second;
}

bool StringValueMapperSimple::Map(
    const StringValue& From, const SimpleValue* To)
{
    StringValue* sv = nullptr;
    if (IsStringValue(To))
        sv = dynamic_cast<StringValue*>(To->Clone());
    auto result = map.insert(std::pair<StringValue,StringValue*>(From, sv));
    if (!result.second)
        delete sv;
    return result.second;
}

StringValue StringValueMapperSimple::operator[](const StringValue& Value) const
{
    auto iter = map.find(Value);
    if (iter != map.cend()) {
        if (iter->second)
            return *(iter->second);
        return StringValue();
    }
    return StringValue(prefix + Value.String() + postfix);
}
