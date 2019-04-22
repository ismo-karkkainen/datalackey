//
//  MemoryOwner.hpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 9.1.18.
//  Copyright © 2018 Ismo Kärkkäinen. All rights reserved.
//
// Licensed under Universal Permissive License. See License.txt.

#ifndef MemoryOwner_hpp
#define MemoryOwner_hpp

#include "DataOwner.hpp"


class MemoryOwner : public DataOwner {
private:
    std::shared_ptr<const RawData> data;
    bool finished;

public:
    MemoryOwner();
    ~MemoryOwner();

    bool Owner() const;
    void Own(bool OwnMemory);
    bool Error() const;
    bool Cacheable() const;
    bool Finished() const;

    bool Append(RawData::ConstIterator From, RawData::ConstIterator To);
    void Finish();

    size_t Size() const;
    std::shared_ptr<const RawData> FullData();

    std::shared_ptr<DataReader> Reader(std::shared_ptr<DataOwner>& Owner);
};


#endif /* MemoryOwner_hpp */
