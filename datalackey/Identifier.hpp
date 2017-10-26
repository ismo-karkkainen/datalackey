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


class Identifier : public SimpleValue {
private:
    // Could use variant but this is most likely sufficient for convenient use.
    // Allow integer or string as identifier.
    bool is_number;
    long long int number;
    std::string string;

public:
    Identifier(long long int N);
    Identifier(const std::string& S);

    bool IsNumber() const;
    long long int Number() const;
    const std::string& String() const;
};


#endif /* Identifier_hpp */
