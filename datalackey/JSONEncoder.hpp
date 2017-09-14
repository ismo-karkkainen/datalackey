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
    // If outputs directly, only appending to Buffer is allowed.
    bool Encode(RawData& Buffer, Structure S);
    bool Encode(RawData& Buffer, const ValueReference& VR);
    // Return an instance of the same class.
    Encoder* CreateSame() const;
    // Return true if Encode always produces output (returns true).
    bool EncodeOutputsDirectly() const;
    const char *const Format() const;
};


#endif /* JSONEncoder_hpp */
