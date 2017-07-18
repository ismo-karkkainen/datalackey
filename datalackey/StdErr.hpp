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

    OutputChannel& operator<<(const std::vector<char>& Buffer);
    void Write(Iterator& Start, Iterator& End);
    void Flush();
};

#endif /* StdErr_hpp */
