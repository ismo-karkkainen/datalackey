//
//  Output.cpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 16.3.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#include "Output.hpp"

ValueReference::~ValueReference() { }
size_t ValueReference::Size() const { return 0; }
const char* ValueReference::Raw() const { return nullptr; }
bool ValueReference::IsChar() const { return false; }
bool ValueReference::IsString() const { return false; }
bool ValueReference::IsInteger() const { return false; }
bool ValueReference::IsSigned() const { return false; }
bool ValueReference::IsNegative() const { return false; }
unsigned long long ValueReference::Absolute() const { return 0ULL; }
bool ValueReference::IsFloat() const { return false; }
std::string ValueReference::String() const { return std::string(); }


Output::~Output() { }

