//
//  StdIn.hpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 24.5.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#ifndef StdIn_hpp
#define StdIn_hpp

#include "InputChannel.hpp"
#include "RawData.hpp"


class StdIn : public InputChannel {
public:
    ~StdIn();
    std::streamsize Read(RawData& Buffer);
    bool Ended();
};


#endif /* StdIn_hpp */
