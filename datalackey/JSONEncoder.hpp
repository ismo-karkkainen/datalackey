//
//  JSONEncoder.hpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 9.5.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#ifndef JSONEncoder_hpp
#define JSONEncoder_hpp

#include "Encoder.hpp"
#include <stack>

class JSONEncoder : public Encoder {
private:
    std::stack<Structure> open;
    std::stack<int> counter;

    void add_separator(RawData& Buffer);

public:
    JSONEncoder();
    ~JSONEncoder();

    bool Encode(RawData& Buffer, Structure S);
    bool Encode(RawData& Buffer, const ValueReference& VR);

    Encoder* Clone() const;

    bool EncodeOutputsDirectly() const;
    const char *const Format() const;
};


#endif /* JSONEncoder_hpp */
