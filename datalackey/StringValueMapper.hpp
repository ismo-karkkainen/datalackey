//
//  StringValueMapper.hpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 23.10.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#ifndef StringValueMapper_hpp
#define StringValueMapper_hpp

#include "StringValue.hpp"


// Base class for StringValueMapper classes.
class StringValueMapper {
public:
    virtual ~StringValueMapper();

    virtual StringValue operator[](const StringValue& Value) const = 0;

    // All set-up can be done using the derived class methods.
};


#endif /* StringValueMapper_hpp */
