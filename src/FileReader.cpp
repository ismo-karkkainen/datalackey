//
//  FileReader.cpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 8.1.18.
//  Copyright © 2018 Ismo Kärkkäinen. All rights reserved.
//
// Licensed under Universal Permissive License. See License.txt.

#include "FileReader.hpp"
#include "Time.hpp"
#include <unistd.h>
#include <fcntl.h>
#include <cassert>
#include <cerrno>
#include <sys/stat.h>


FileReader::FileReader(
    const std::string& Filename, std::shared_ptr<DataOwner> Owner)
    : name(Filename), in_error(false), total_read(0), owner(Owner)
{
    assert(owner->Finished());
}

FileReader::~FileReader() { }

bool FileReader::Error() const {
    return in_error;
}

size_t FileReader::Size() const {
    return owner->Size();
}

std::shared_ptr<const RawData> FileReader::Read(size_t SuggestedBlockSize) {
    if (in_error || total_read == Size()) {
        if (fd)
            fd->Close();
        return nullptr;
    }
    if (!fd || fd->Closed()) {
        fd.reset(FileDescriptor::Open(name.c_str()));
        if (fd == nullptr) {
            in_error = true;
            return nullptr;
        }
    }
    if (Size() - total_read < 2 * SuggestedBlockSize)
        SuggestedBlockSize = Size() - total_read;
    RawData* result = new RawData();
    char* buffer = result->Get(SuggestedBlockSize);
    int got = read(fd->Descriptor(), buffer, SuggestedBlockSize);
    total_read += got;
    if (got != SuggestedBlockSize) {
        delete result;
        result = nullptr;
        in_error = true;
    }
    return std::shared_ptr<const RawData>(result);
}
