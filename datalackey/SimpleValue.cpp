//
//  SimpleValue.cpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 25.10.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//
// Licensed under Universal Permissive License. See License.txt.

#include "SimpleValue.hpp"

SimpleValue::~SimpleValue() { }

bool SimpleValue::operator<(const SimpleValue& S) const {
    if (Number() < S.Number())
        return true;
    if (S.Number() < Number())
        return false;
    if (String() < S.String())
        return true;
    // Numbers were equal and string was not less so false.
    return false;
}
