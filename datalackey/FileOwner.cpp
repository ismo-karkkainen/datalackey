//
//  FileOwner.cpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 8.1.18.
//  Copyright © 2018 Ismo Kärkkäinen. All rights reserved.
//

#include "FileOwner.hpp"
#include "FileReader.hpp"
#include "Time.hpp"
#include <unistd.h>
#include <fcntl.h>
#include <cassert>
#include <cerrno>
#include <sys/stat.h>


bool FileOwner::make_dirs(const std::string& Root, const std::string& dir) {
    return true;
}

bool FileOwner::append(RawData::ConstIterator From, RawData::ConstIterator To) {
    if (fd == -1) {
        std::string root(full, 0, full.size() - partial.size());
        if (!make_dirs(root, partial))
            return false;
        errno = 0;
        // Get mode from somewhere.
        fd = open(full.c_str(), O_WRONLY | O_CREAT | O_EXCL | O_CLOEXEC,
            S_IRUSR | S_IWUSR);
        if (fd == -1)
            return false;
        present = true;
    }
    ssize_t written = write(fd, &(*From), To - From);
    if (written >= 0)
        length += written;
    return written != -1;
}

FileOwner::FileOwner(
    const std::string& Root, const std::string& Filename, bool Owned)
    : owner(Owned), present(true), finished(true), in_error(false),
    full(Root + Filename), partial(Filename), fd(-1), length(0)
{
    // File exists so we need to get the length.
    fd = open(full.c_str(), O_RDONLY | O_CLOEXEC);
    if (fd == -1) {
        in_error = true;
        // Should report to book-keeping but it should be evident when read.
        return;
    }
    struct stat info;
    if (-1 == fstat(fd, &info)) {
        close(fd);
        in_error = true;
    }
    length = info.st_size;
}

FileOwner::FileOwner(const std::string& Root, const std::string& Filename)
    : owner(true), present(false), finished(false), in_error(false),
    full(Root + Filename), partial(Filename), fd(-1), length(0)
{ }

FileOwner::~FileOwner() {
    if (fd != -1)
        close(fd);
    if (owner && present) {
        unlink(full.c_str());
    }
}

bool FileOwner::Owner() const {
    return owner;
}

void FileOwner::Own(bool OwnFile) {
    assert(fd == -1);
    owner = OwnFile;
}

bool FileOwner::Error() const {
    return in_error;
}

bool FileOwner::Cacheable() const {
    return true;
}

bool FileOwner::Finished() const {
    return finished;
}

bool FileOwner::Append(RawData::ConstIterator From, RawData::ConstIterator To) {
    assert(owner);
    assert(!finished);
    if (!in_error)
        in_error = !append(From, To);
    return !in_error;
}

void FileOwner::Finish() {
    assert(owner);
    assert(!finished);
    if (fd != -1)
        close(fd);
    fd = -1;
    finished = true;
    // File may be empty because input contains bad data before first write.
}

size_t FileOwner::Size() const {
    return length;
}

std::shared_ptr<const RawData> FileOwner::FullData() {
    return nullptr;
}

std::shared_ptr<DataReader> FileOwner::Reader(std::shared_ptr<DataOwner>& Owner)
{
    return std::shared_ptr<DataReader>(new FileReader(Owner));
}
