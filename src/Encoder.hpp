//
//  Encoder.hpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 12.4.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//
// Licensed under Universal Permissive License. See License.txt.

#ifndef Encoder_hpp
#define Encoder_hpp

#include "ValueReference.hpp"
#include "Structure.hpp"
#include "RawData.hpp"
#include <vector>

// Base class that takes ValueReference and returns representation of it
// in the given format. Output is a byte array so that everything is covered.
// Provides values for array and dictionary start and end. Or if something needs
// to contain the array length, then would take values and produce output when
// ready. Hence begin and end are values themselves.

class Encoder {
public:
    virtual ~Encoder();
    // If outputs directly, only appending to Buffer is allowed.
    virtual bool Encode(RawData& Buffer, Structure S) = 0;
    virtual bool Encode(RawData& Buffer, const ValueReference& VR) = 0;
    // Return an instance of the same class.
    virtual Encoder* Clone() const = 0;
    // Return true if Encode always produces output (returns true).
    virtual bool EncodeOutputsDirectly() const = 0;

    virtual const char *const Format() const = 0;
};


#endif /* Encoder_hpp */
