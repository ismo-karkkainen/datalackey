//
//  StringValueMapperSimple.cpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 23.10.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//
// Licensed under Universal Permissive License. See License.txt.

#include "StringValueMapperSimple.hpp"


StringValueMapperSimple::StringValueMapperSimple(
    const std::string& Prefix, const std::string& Postfix)
    : prefix(Prefix), postfix(Postfix)
{ }

bool StringValueMapperSimple::Map(
    const StringValue& From, std::shared_ptr<SimpleValue>& To)
{
    std::shared_ptr<StringValue> sv;
    if (IsStringValue(To.get()))
        sv = std::shared_ptr<StringValue>(
            dynamic_cast<StringValue*>(To->Clone()));
    auto result = map.insert(
        std::pair<StringValue,std::shared_ptr<StringValue>>(From, sv));
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
