//
//  Label.cpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 25.10.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#include "Label.hpp"


Label::Label(const std::string& Name)
    : label(Name)
{ }

bool Label::IsNumber() const {
    return false;
}

long long int Label::Number() const {
    return 0;
}

const std::string& Label::String() const {
    return label;
}
