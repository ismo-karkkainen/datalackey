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
    std::shared_ptr<const RawData> FullData();

    std::shared_ptr<DataReader> Reader(std::shared_ptr<DataOwner>& Owner);

    RawData* Raw();
};


#endif /* RawDataOwner_hpp */
