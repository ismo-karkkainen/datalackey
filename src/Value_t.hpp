//
//  Value_t.hpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 10.4.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//
// Licensed under Universal Permissive License. See License.txt.

#ifndef Value_t_hpp
#define Value_t_hpp

#include "Output.hpp"
#include <sstream>
#include <typeinfo>

template<typename T>
class ValueRef : public ValueReference {
private:
    const T& value;
public:
    ValueRef(const T& Value) : value(Value) { }
    ~ValueRef() { }
    size_t Size() const { return sizeof(T); }
    const char* Raw() const { return reinterpret_cast<const char*>(&value); }
    bool IsChar() const;
    bool IsString() const;
    std::string String() const;
};

template<typename T>
bool ValueRef<T>::IsChar() const {
    return
        (typeid(T) == typeid(signed char)) || (typeid(T) == typeid(unsigned char)) ||
#if !defined(__GNUC__)
        (typeid(T) == typeid(signed wchar_t)) || (typeid(T) == typeid(unsigned wchar_t)) ||
#else
        (typeid(T) == typeid(wchar_t)) ||
#endif
        (typeid(T) == typeid(char16_t)) ||
        (typeid(T) == typeid(char32_t));
}

template<typename T>
bool ValueRef<T>::IsString() const {
    return (typeid(T) == typeid(std::string)) ||
        (typeid(T) == typeid(signed char*)) || (typeid(T) == typeid(unsigned char*)) ||
#if !defined(__GNUC__)
        (typeid(T) == typeid(signed wchar_t*)) || (typeid(T) == typeid(unsigned wchar_t*)) ||
#else
        (typeid(T) == typeid(wchar_t*)) ||
#endif
        (typeid(T) == typeid(char16_t*)) ||
        (typeid(T) == typeid(char32_t*));
}

template<typename T>
std::string ValueRef<T>::String() const {
    std::stringstream s;
    s << value;
    return s.str();
}


#endif /* Value_t_hpp */
