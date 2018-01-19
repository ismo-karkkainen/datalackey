//
//  RawDataOwner.hpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 10.1.18.
//  Copyright © 2018 Ismo Kärkkäinen. All rights reserved.
//

#ifndef RawDataOwner_hpp
#define RawDataOwner_hpp

#include "DataOwner.hpp"


class RawDataOwner : public DataOwner {
private:
    std::shared_ptr<const RawData> data;
    bool finished;
    bool read;

public:
    RawDataOwner();
    ~RawDataOwner();

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

    RawData* Raw();
};


#endif /* RawDataOwner_hpp */
