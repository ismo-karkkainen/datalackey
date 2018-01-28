//
//  OutputItem.hpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 6.12.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#ifndef OutputItem_hpp
#define OutputItem_hpp

#include "Structure.hpp"
#include "ValueReference.hpp"
#include "RawData.hpp"


// Base class for internal buffer writer and discarder classes.
class OutputItem {
public:
    virtual ~OutputItem();
    virtual OutputItem& operator<<(Structure S) = 0;
    virtual OutputItem& operator<<(const ValueReference& VR) = 0;
    virtual void Write(
        RawData::ConstIterator Start, RawData::ConstIterator End) = 0;
};

// For a case when output is meant to be discarded, like with Null identifier.
class OutputItemDiscarder : public OutputItem {
public:
    ~OutputItemDiscarder();
    OutputItem& operator<<(Structure S);
    OutputItem& operator<<(const ValueReference& VR);
    void Write(RawData::ConstIterator Start, RawData::ConstIterator End);
};


#endif /* OutputItem_hpp */
