//
//  StdErr.hpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 12.4.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#ifndef StdErr_hpp
#define StdErr_hpp

#include "OutputChannel.hpp"


class StdErr : public OutputChannel {
public:
    ~StdErr();

    OutputChannel& operator<<(const RawData& Buffer);
    void Write(RawData::Iterator& Start, RawData::Iterator& End);
    void Flush();
};

#endif /* StdErr_hpp */
