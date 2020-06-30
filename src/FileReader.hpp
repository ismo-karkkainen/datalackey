//
//  FileReader.hpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 21.1.18.
//  Copyright © 2018 Ismo Kärkkäinen. All rights reserved.
//
// Licensed under Universal Permissive License. See License.txt.

#ifndef FileReader_hpp
#define FileReader_hpp

#include "DataReader.hpp"
#include "DataOwner.hpp"
#include "FileDescriptor.hpp"
#include <string>
#include <memory>


class FileReader : public DataReader {
private:
    std::string name;
    mutable std::unique_ptr<FileDescriptor> fd;
    bool in_error;
    size_t total_read;

    // Just to ensure file does not get deleted before we have finshed reading.
    std::shared_ptr<DataOwner> owner;

public:
    FileReader(const std::string& Filename, std::shared_ptr<DataOwner> Owner);
    ~FileReader();

    bool Error() const;

    size_t Size() const;
    std::shared_ptr<const RawData> Read(size_t SuggestedBlockSize);
};


#endif /* FileReader_hpp */
