//
//  Literal.cpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 11.4.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#include "Literal.hpp"
#include <cstring>


Literal::~Literal() { }

size_t Literal::Size() const {
    return strlen(value);
}

const char* Literal::Raw() const {
    return value;
}

bool Literal::IsString() const {
    return true;
}

std::string Literal::String() const {
    return std::string(value);
}
