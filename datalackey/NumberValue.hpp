//
//  NumberValue.hpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 24.10.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#ifndef NumberValue_hpp
#define NumberValue_hpp

#include "SimpleValue.hpp"


class NumberValue : public SimpleValue {
private:
    long long int number;
    std::string string;

    NumberValue(long long int N, const std::string& S);

public:
    NumberValue(long long int N);

    long long int Number() const;
    const std::string& String() const;
    SimpleValue* Clone() const;
};


extern bool IsNumberValue(const SimpleValue* S);

#endif /* NumberValue_hpp */
