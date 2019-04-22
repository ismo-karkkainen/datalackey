//
//  NullValue.cpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 12.11.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//
// Licensed under Universal Permissive License. See License.txt.

#include "NullValue.hpp"


// Avoid returning a reference to local temporary.
static std::string empty;

long long int NullValue::Number() const {
    return 0;
}

const std::string& NullValue::String() const {
    return empty;
}

SimpleValue* NullValue::Clone() const {
    return new NullValue();
}

bool IsNullValue(const SimpleValue* S) {
    const NullValue* null(dynamic_cast<const NullValue*>(S));
    return null != nullptr;
}
