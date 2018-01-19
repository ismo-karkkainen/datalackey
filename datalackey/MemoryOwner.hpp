//
//  MemoryOwner.hpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 9.1.18.
//  Copyright © 2018 Ismo Kärkkäinen. All rights reserved.
//

#ifndef MemoryOwner_hpp
#define MemoryOwner_hpp

#include "DataOwner.hpp"


class MemoryOwner : public DataOwner {
private:
    std::shared_ptr<const RawData> data;
    bool finished;
    bool read;

public:
    MemoryOwner();
    MemoryOwner(const MemoryOwner& M);
    ~MemoryOwner();

    bool Owner() const;
    void Own(bool OwnMemory);
    bool Error() const;
    bool Cacheable() const;
    bool Finished() const;

    bool Append(RawData::ConstIterator From, RawData::ConstIterator To);
    void Finish();

    size_t Size() const;
    bool StartRead();
    std::shared_ptr<const RawData> Read(size_t SuggestedBlockSize);
    void FinishRead();
};


#endif /* MemoryOwner_hpp */
