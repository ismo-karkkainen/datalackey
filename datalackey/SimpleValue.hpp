//
//  SimpleValue.hpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 25.10.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#ifndef SimpleValue_hpp
#define SimpleValue_hpp

#include <string>


// Base class for strings and numbers. Sort of simple variant class.
class SimpleValue {
public:
    virtual ~SimpleValue();

    virtual bool IsNumber() const = 0;
    virtual long long int Number() const = 0;
    virtual const std::string& String() const = 0;
};


#endif /* SimpleValue_hpp */
