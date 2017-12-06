//
//  StringValueMapperSimple.hpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 23.10.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#ifndef StringValueMapperSimple_hpp
#define StringValueMapperSimple_hpp

#include "StringValueMapper.hpp"
#include <string>
#include <map>


class StringValueMapperSimple : public StringValueMapper {
private:
    std::map<StringValue, StringValue*> map;
    std::string prefix, postfix;

public:
    StringValueMapperSimple(
        const std::string& Prefix, const std::string& Postfix);
    ~StringValueMapperSimple();

    bool Map(const StringValue& From, const SimpleValue* To);

    StringValue operator[](const StringValue& Value) const;
};


#endif /* StringValueMapperSimple_hpp */
