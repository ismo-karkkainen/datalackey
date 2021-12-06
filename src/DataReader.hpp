//
//  DataReader.hpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 21.1.18.
//  Copyright © 2018 Ismo Kärkkäinen. All rights reserved.
//
// Licensed under Universal Permissive License. See License.txt.

#ifndef DataReader_hpp
#define DataReader_hpp

#include "RawData.hpp"
#include <memory>
#include <cstddef>


// Base class for data owning classes.
class DataReader {
public:
    DataReader& operator=(const DataReader&) = delete;
    DataReader(const DataReader&) = delete;

    DataReader();
    virtual ~DataReader();

    virtual bool Error() const = 0;

    virtual size_t Size() const = 0;
    virtual std::shared_ptr<const RawData> Read(size_t SuggestedBlockSize) = 0;
};


#endif /* DataReader_hpp */
