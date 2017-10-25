//
//  Identifier.hpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 24.10.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#ifndef Identifier_hpp
#define Identifier_hpp

#include "SimpleValue.hpp"
#include <string>


class Identifier {
private:
    // Could use variant but this is most likely sufficient for convenient use.
    // Allow integer or string as identifier.
    bool is_number;
    long long int number;
    std::string string;

public:
    Identifier(long long int N) : is_number(true), number(N) { }
    Identifier(const std::string& S) : is_number(false), string(S) { }

    bool IsNumber() const { return is_number; }
    long long int Number() const { return number; }
    const std::string& String() const { return string; }
};


#endif /* Identifier_hpp */
