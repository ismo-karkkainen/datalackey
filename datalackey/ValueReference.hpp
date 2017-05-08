//
//  ValueReference.hpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 16.3.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#ifndef ValueReference_hpp
#define ValueReference_hpp

#include <string>


class ValueReference {
public:
    virtual ~ValueReference();
    // Methods that return information about the value reference for Encoder.
    // String covers needs of JSON, the rest should be enough for CBOR.

    virtual size_t Size() const; // For simple types, sizeof.
    virtual const char* Raw() const; // Pointer to simple type data as is.
    virtual bool IsChar() const;
    virtual bool IsString() const;
    virtual bool IsInteger() const;
    virtual bool IsSigned() const;
    virtual bool IsNegative() const;
    virtual unsigned long long Absolute() const;
    virtual bool IsFloat() const;
    virtual std::string String() const;
};

#endif /* ValueReference_hpp */
