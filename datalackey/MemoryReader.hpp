//
//  MemoryReader.hpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 21.1.18.
//  Copyright © 2018 Ismo Kärkkäinen. All rights reserved.
//
// Licensed under Universal Permissive License. See License.txt.

#ifndef MemoryReader_hpp
#define MemoryReader_hpp

#include "DataReader.hpp"
#include "DataOwner.hpp"
#include <memory>


// Use this when DataOwner object returns data via FullData().

class MemoryReader : public DataReader {
private:
    std::shared_ptr<DataOwner> source;
    bool finished;

public:
    MemoryReader(std::shared_ptr<DataOwner>& Source);
    ~MemoryReader();

    bool Error() const;

    size_t Size() const;
    std::shared_ptr<const RawData> Read(size_t SuggestedBlockSize);
};


#endif /* MemoryReader_hpp */
