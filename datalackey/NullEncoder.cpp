//
//  NullEncoder.cpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 29.1.18.
//  Copyright © 2018 Ismo Kärkkäinen. All rights reserved.
//

#include "NullEncoder.hpp"


NullEncoder::NullEncoder() { }

NullEncoder::~NullEncoder() { }

bool NullEncoder::Encode(RawData& Buffer, Structure S) {
    return true;
}

bool NullEncoder::Encode(RawData& Buffer, const ValueReference& VR) {
    return true;
}

Encoder* NullEncoder::Clone() const {
    return new NullEncoder();
}

bool NullEncoder::EncodeOutputsDirectly() const {
    return true;
}

const char *const NullEncoder::Format() const {
    return nullptr;
}
