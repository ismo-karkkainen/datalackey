//
//  Literal.hpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 11.4.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#ifndef Literal_hpp
#define Literal_hpp

#include "ValueReference.hpp"


class Literal : public ValueReference {
private:
    const char *const value;
public:
    Literal(const char *const Value) : value(Value) { }
    ~Literal();
    size_t Size() const;
    const char* Raw() const;
    bool IsString() const;
    std::string String() const;
};

#endif /* Literal_hpp */