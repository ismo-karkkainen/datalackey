//
//  FileReader.cpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 8.1.18.
//  Copyright © 2018 Ismo Kärkkäinen. All rights reserved.
//

#include "FileReader.hpp"
#include "Time.hpp"
#include <unistd.h>
#include <fcntl.h>
#include <cassert>
#include <cerrno>
#include <sys/stat.h>


FileReader::FileReader(
    const std::string& Filename, std::shared_ptr<DataOwner> Owner)
    : name(Filename), fd(-1), in_error(false), total_read(0), owner(Owner)
{
    assert(owner->Finished());
}

FileReader::~FileReader() {
    if (fd != -1)
        close(fd);
}

bool FileReader::Error() const {
    return in_error;
}

size_t FileReader::Size() const {
    return owner->Size();
}

std::shared_ptr<const RawData> FileReader::Read(size_t SuggestedBlockSize) {
    if (in_error || total_read == Size()) {
        if (fd != -1) {
            close(fd);
            fd = -1;
        }
        return nullptr;
    }
    if (fd == -1) {
        fd = open(name.c_str(), O_RDONLY | O_CLOEXEC);
        if (fd == -1) {
            in_error = true;
            return nullptr;
        }
    }
    if (Size() - total_read < 2 * SuggestedBlockSize)
        SuggestedBlockSize = Size() - total_read;
    RawData* result = new RawData();
    char* buffer = result->Get(SuggestedBlockSize);
    int got = read(fd, buffer, SuggestedBlockSize);
    total_read += got;
    if (got != SuggestedBlockSize) {
        delete result;
        result = nullptr;
        in_error = true;
    }
    return std::shared_ptr<const RawData>(result);
}
