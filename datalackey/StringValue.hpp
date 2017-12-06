//
//  StringValue.hpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 24.10.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#ifndef StringValue_hpp
#define StringValue_hpp

#include "SimpleValue.hpp"


class StringValue : public SimpleValue {
private:
    std::string string;

public:
    StringValue(const std::string& Name = std::string());

    long long int Number() const;
    const std::string& String() const;
    SimpleValue* Clone() const;

    inline operator std::string() const { return string; }

    inline bool operator<(const StringValue& S) const { return string < S.string; }
    inline bool operator!=(const StringValue& S) const { return string != S.string; }
};


extern bool IsStringValue(const SimpleValue* S);

#endif /* StringValue_hpp */
