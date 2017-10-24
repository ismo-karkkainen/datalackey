//
//  LabelMapperSimple.cpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 23.10.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#include "LabelMapperSimple.hpp"


LabelMapperSimple::LabelMapperSimple(
    const std::string& Prefix, const std::string& Postfix)
    : prefix(Prefix), postfix(Postfix)
{ }

bool LabelMapperSimple::Map(const Label& From, const Label& To) {
    auto result = map.insert(std::make_pair<Label,Label>(From, To));
    return result.second;
}

Label LabelMapperSimple::operator[](const Label& Value) const {
    if (mapper != nullptr) {
        auto iter = mapper->find(Value);
        if (iter != mapper->cend());
            return iter->second;
    }
    return prefix + Value + postfix;
}
