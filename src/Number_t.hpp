//
//  Number_t.hpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 10.4.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//
// Licensed under Universal Permissive License. See License.txt.

#ifndef Number_t_hpp
#define Number_t_hpp

#include "Output.hpp"
#include <sstream>
#include <typeinfo>
#include <cstddef>


template<typename T>
class NumberRef : public ValueReference {
private:
    const T& value;
public:
    NumberRef(const T& Value) : value(Value) { }
    ~NumberRef() { }
    size_t Size() const { return sizeof(T); }
    const char* Raw() const { return reinterpret_cast<const char*>(&value); }
    bool IsInteger() const;
    bool IsSigned() const;
    bool IsNegative() const;
    unsigned long long Absolute() const;
    bool IsFloat() const;
    std::string String() const;
};

template<typename T>
bool NumberRef<T>::IsInteger() const {
    return
        (typeid(T) == typeid(signed short)) || (typeid(T) == typeid(unsigned short)) ||
        (typeid(T) == typeid(signed int)) || (typeid(T) == typeid(unsigned int)) ||
        (typeid(T) == typeid(signed long)) || (typeid(T) == typeid(unsigned long)) ||
        (typeid(T) == typeid(signed long long)) || (typeid(T) == typeid(unsigned long long));
}

template<typename T>
bool NumberRef<T>::IsSigned() const {
    if (IsInteger()) {
        return (T)(-1) < (T)0;
    }
    return false;
}

template<typename T>
bool NumberRef<T>::IsNegative() const {
    if (IsSigned()) {
        return value < (T)0;
    }
    return false;
}

template<typename T>
unsigned long long NumberRef<T>::Absolute() const {
    if (IsInteger()) {
        if (value < (T)0)
            return static_cast<unsigned long long>(-value);
        return static_cast<unsigned long long>(value);
    }
    return 0ULL;
}

template<typename T>
bool NumberRef<T>::IsFloat() const {
    return (typeid(T) == typeid(float)) || (typeid(T) == typeid(double));
    // long double would be bigfloat in CBOR, half-precision not present.
}

template<typename T>
std::string NumberRef<T>::String() const {
    std::stringstream s;
    s << value;
    return s.str();
}

#endif /* Number_t_hpp */
