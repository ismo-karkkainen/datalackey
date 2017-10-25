//
//  Identifier.cpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 25.10.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#include "Identifier.hpp"


Identifier::Identifier(long long int N)
    : is_number(true), number(N)
{ }

Identifier::Identifier(const std::string& S)
    : is_number(false), string(S)
{ }

bool Identifier::IsNumber() const {
    return is_number;
}

long long int Identifier::Number() const {
    return number;
}

const std::string& Identifier::String() const {
    return string;
}
