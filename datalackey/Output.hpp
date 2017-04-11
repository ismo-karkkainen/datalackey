//
//  Output.hpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 16.3.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#ifndef Output_hpp
#define Output_hpp

// This is utility class that uses transfer and format as needed.
// When intermediate storage of messages is needed, this handles it.
// Or, the derived class handles it.



// This has to be transfer and representation, not one class. Otherwise
// simple as everything goes into one place except for standard input/output
// case with text encoding where the error message type needs to go to stderr.
// Hence something from encoding has to pass through along with result of the
// encoding.
// The encoding result could have some field that contains enough information
// for stdio. So should the encoder hide transfer so that they can be married
// for the special case pair? Encoder should know when to send onwards.
// The encoder can be initialized with transfer and the text encoder requires
// stdio transfer and no-one else knows about the transfer.
// For other pairs, allow initialization using base transfer class and thus
// get various combinations done.

#include <string>

enum MessageType {
    Message = 1,
    Warning,
    Error,
    Array, // Each value is a ValueReference or array/dictionary.
    Dictionary, // Presumes key/value pairs in that order. One value each.
    End // End of any of the previous.
};

// The Message to Error should be treated parallele to everything else and they
// can have some default arrays to append to, but otherwise we must be able to
// output dictionaries and maybe lists, in fact we should be able to output an
// entire JSON-object or similar structured data. For normal stdout/stderr
// output these are just formatting.
// For other data we should construct structured data and on flush, "close" it
// and send out.
// Whether to output right away or until flushed is internal implementation.
// Depends on the format. Hence no need to mock stream with JSON when all
// output is sent at once.


class ValueReference {
public:
    virtual ~ValueReference();
    // Methods that return information about the value reference.
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

    // How about intermediate class that stores the value reference and is
    // a template? But then you'd need that class to handle some kind of
    // union or list of known values and we are back at the beginning.
    // So declare the alternatives directly?
    // Or the template class would offer enough data about value for
    // the encodings? So it could convert to string, or offer sizeof etc.
    // Although with integers, it'd have to know the values in question.
    // Is some preprocessor trickery possible, or handle only the limited
    // number of integer types?
};


class Output {

public:
    virtual ~Output();

    virtual Output& operator<<(MessageType type) = 0;
    virtual Output& operator<<(const ValueReference& VR) = 0;
    virtual Output& Flush() = 0;
};

#endif /* Output_hpp */
