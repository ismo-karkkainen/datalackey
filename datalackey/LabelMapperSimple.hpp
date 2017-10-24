//
//  LabelMapperSimple.hpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 23.10.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#ifndef LabelMapperSimple_hpp
#define LabelMapperSimple_hpp

#include "LabelMapper.hpp"
#include <string>
#include <map>


class LabelMapperSimple {
private:
    std::map<Label, Label> map;
    std::string prefix, postfix;

public:
    StrignMapperSimple(const std::string& Prefix, const std::string& Postfix);
    ~LabelMapperSimple();

    bool Map(const Label& From, const Label& To);

    Label operator[](const Label& Value) const;
};


#endif /* LabelMapperSimple_hpp */
