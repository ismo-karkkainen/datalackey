//
//  StringValue.cpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 25.10.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#include "StringValue.hpp"


StringValue::StringValue(const std::string& Name)
    : string(Name)
{ }

long long int StringValue::Number() const {
    return 0;
}

const std::string& StringValue::String() const {
    return string;
}

SimpleValue* StringValue::Clone() const {
    return new StringValue(string);
}

bool IsStringValue(const SimpleValue* S) {
    const StringValue* str(dynamic_cast<const StringValue*>(S));
    return str != nullptr;
}
