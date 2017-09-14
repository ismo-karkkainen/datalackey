//
//  StdOut.hpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 12.4.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#ifndef StdOut_hpp
#define StdOut_hpp

#include "OutputChannel.hpp"


class StdOut : public OutputChannel {
public:
    ~StdOut();

    OutputChannel& operator<<(const RawData& Buffer);
    void Write(RawData::ConstIterator& Start, RawData::ConstIterator& End);
    void Flush();
};

#endif /* StdOut_hpp */
