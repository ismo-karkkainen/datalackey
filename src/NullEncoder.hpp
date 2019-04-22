//
//  NullEncoder.hpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 29.1.18.
//  Copyright © 2018 Ismo Kärkkäinen. All rights reserved.
//
// Licensed under Universal Permissive License. See License.txt.

#ifndef NullEncoder_hpp
#define NullEncoder_hpp

#include "Encoder.hpp"


class NullEncoder : public Encoder {
public:
    NullEncoder();
    ~NullEncoder();

    bool Encode(RawData& Buffer, Structure S);
    bool Encode(RawData& Buffer, const ValueReference& VR);

    Encoder* Clone() const;

    bool EncodeOutputsDirectly() const;
    const char *const Format() const;
};


#endif /* NullEncoder_hpp */
