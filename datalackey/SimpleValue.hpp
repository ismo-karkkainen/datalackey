//
//  SimpleValue.hpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 25.10.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//
// Licensed under Universal Permissive License. See License.txt.

#ifndef SimpleValue_hpp
#define SimpleValue_hpp

#include <string>
#include <functional>


// Base class for strings and numbers. Sort of simple variant class.
class SimpleValue {
public:
    virtual ~SimpleValue();

    virtual long long int Number() const = 0;
    virtual const std::string& String() const = 0;
    virtual SimpleValue* Clone() const = 0;

    bool operator<(const SimpleValue& S) const;
};

namespace std {
template <> struct less<SimpleValue*> {
    bool operator() (const SimpleValue* A, const SimpleValue* B) const {
        return *A < *B;
    }
};
}

#endif /* SimpleValue_hpp */
