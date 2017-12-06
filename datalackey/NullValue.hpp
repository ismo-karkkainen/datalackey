//
//  NullValue.hpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 12.11.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#ifndef NullValue_hpp
#define NullValue_hpp

#include "SimpleValue.hpp"


class NullValue : public SimpleValue {
public:
    long long int Number() const;
    const std::string& String() const;
    SimpleValue* Clone() const;
};


extern bool IsNullValue(const SimpleValue* S);

#endif /* NullValue_hpp */
