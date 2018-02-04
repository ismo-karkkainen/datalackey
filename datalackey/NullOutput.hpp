//
//  NullOutput.hpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 29.1.18.
//  Copyright © 2018 Ismo Kärkkäinen. All rights reserved.
//

#ifndef NullOutput_hpp
#define NullOutput_hpp

#include "OutputChannel.hpp"


class NullOutput : public OutputChannel {
private:
    bool closed;

public:
    NullOutput();
    ~NullOutput();

    size_t Write(RawData::ConstIterator& Start, RawData::ConstIterator& End);
    void Flush();
    void Close();
    bool Failed() const;
    bool Closed() const;
};


#endif /* NullOutput_hpp */
