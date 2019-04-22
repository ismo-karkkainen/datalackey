//
//  StringValueMapperSimple.hpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 23.10.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//
// Licensed under Universal Permissive License. See License.txt.

#ifndef StringValueMapperSimple_hpp
#define StringValueMapperSimple_hpp

#include "StringValueMapper.hpp"
#include <string>
#include <map>
#include <memory>


class StringValueMapperSimple : public StringValueMapper {
private:
    std::map<StringValue, std::shared_ptr<StringValue>> map;
    std::string prefix, postfix;

public:
    StringValueMapperSimple(
        const std::string& Prefix, const std::string& Postfix);

    bool Map(const StringValue& From, std::shared_ptr<SimpleValue>& To);

    StringValue operator[](const StringValue& Value) const;
};


#endif /* StringValueMapperSimple_hpp */
