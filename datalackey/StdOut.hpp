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

    OutputChannel& operator<<(const std::vector<char>& Buffer);
    void Flush();
};

#endif /* StdOut_hpp */
