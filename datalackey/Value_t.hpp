//
//  Value_t.hpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 10.4.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

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
        (typeid(T) == typeid(signed wchar_t)) || (typeid(T) == typeid(unsigned wchar_t)) ||
        (typeid(T) == typeid(char16_t)) ||
        (typeid(T) == typeid(char32_t));
}

template<typename T>
bool ValueRef<T>::IsString() const {
    return (typeid(T) == typeid(std::string)) ||
        (typeid(T) == typeid(signed char*)) || (typeid(T) == typeid(unsigned char*)) ||
        (typeid(T) == typeid(signed wchar_t*)) || (typeid(T) == typeid(unsigned wchar_t*)) ||
        (typeid(T) == typeid(char16_t*)) ||
        (typeid(T) == typeid(char32_t*));
}

template<typename T>
std::string ValueRef<T>::String() const {
    std::stringstream s;
    s << value;
    return s.str();
}

// Output receiving a value needs knowledge of the encoding to choose what to
// pull out.
// Or maybe the outptu passes the value representation directly to encoding and
// the encoding chooses the representation.
// Maybe it is the encoder that is given and output is hidden behind that and
// value goes to encoder. And if we have stdout/stderr encoder then the
// structural information just handles information about indentation and
// the output deals with all stuff.
// Except the cases where information comes out of order, something needs to
// store values to output in proper order and proper arrays.
// So output object uses encoding internally to get stuff to output.
// Output deals with storage as needed, encoding is the format, then some
// stream class is the transfer.


    // Then the base class has method to handle the generic class? But there
    // should be only one class to handle, so the templates should have a base
    // class with virtual functions.
    // Maybe templates for std::string or other more complex types? Or pass
    // the c_str() as value?
    // typeid(T) and typeid(type) seem comparable.
    // For integer types, unsigned could be checked with 0 <= (T)(-1).
    // There aren't that many integers, though, I presume. And for CBOR,
    // if not in -24 <= 23 then it's actual value. Maybe use sizeof to
    // determine if larger range should be checked at all.
    // Have to check a CBOR encoder just to see what they handle and need.

#endif /* Value_t_hpp */
