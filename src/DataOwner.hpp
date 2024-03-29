//
//  DataOwner.hpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 8.1.18.
//  Copyright © 2018 Ismo Kärkkäinen. All rights reserved.
//
// Licensed under Universal Permissive License. See License.txt.

#ifndef DataOwner_hpp
#define DataOwner_hpp

#include "RawData.hpp"
#include "DataReader.hpp"
#include <memory>
#include <cstddef>


// Base class for data owning classes.
class DataOwner {
public:
    DataOwner& operator=(const DataOwner&) = delete;
    DataOwner(const DataOwner&) = delete;

    DataOwner();
    virtual ~DataOwner();

    virtual bool Owner() const = 0;
    virtual void Own(bool OwnData) = 0; // Set ownership status.
    virtual bool Error() const = 0;
    virtual bool Cacheable() const = 0;
    virtual bool Finished() const = 0; // Indicate if no appending in process.

    virtual bool Append(RawData::ConstIterator From, RawData::ConstIterator To) = 0;
    virtual void Finish() = 0; // Can not append after this.

    virtual size_t Size() const = 0;
    virtual std::shared_ptr<const RawData> FullData() = 0;

    virtual std::shared_ptr<DataReader> Reader(
        std::shared_ptr<DataOwner>& Owner) = 0;
};


class DataOwnerGenerator {
public:
    virtual ~DataOwnerGenerator();

    virtual DataOwner* Generate() = 0;
};

#endif /* DataOwner_hpp */
