//
//  NumberValue.cpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 25.10.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#include "NumberValue.hpp"
#include <sstream>


NumberValue::NumberValue(long long int N, const std::string& S)
    : number(N), string(S)
{ }

NumberValue::NumberValue(long long int N)
    : number(N)
{
    std::stringstream ss;
    ss << number;
    string = ss.str();
}

long long int NumberValue::Number() const {
    return number;
}

const std::string& NumberValue::String() const {
    return string;
}

SimpleValue* NumberValue::Clone() const {
    return new NumberValue(number, string);
}

bool IsNumberValue(const SimpleValue* S) {
    const NumberValue* num(dynamic_cast<const NumberValue*>(S));
    return num != nullptr;
}
