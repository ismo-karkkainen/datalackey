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
#include "File.hpp"
#include <unistd.h>
#include <fcntl.h>
#include <cassert>
#include <cerrno>
#include <sys/stat.h>


std::set<size_t> FileOwner::freed;
std::set<size_t> FileOwner::used;
std::mutex FileOwner::index_mutex;


bool FileOwner::make_dirs(const std::string& Root, const std::string& dir) {
    mode_t dirmode = mode | S_IXUSR;
    if (mode | S_IRGRP)
        dirmode |= S_IXGRP;
    if (mode | S_IXOTH)
        dirmode |= S_IXOTH;
    std::string existing(Root);
    std::string todo(dir);
    size_t pos = todo.find(Separator);
    while (pos != std::string::npos) {
        existing += std::string(todo, 0, pos);
        todo = std::string(todo, pos + 1);
        errno = 0;
        int fd = open(existing.c_str(), O_RDONLY | O_CLOEXEC);
        int err = errno;
        if (fd == -1) {
            // Does not exist, make a directory unless other error.
            switch (err) {
            case EACCES:
            case ENOTDIR:
            case ELOOP:
                return false;
            default:
                break;
            }
            if (-1 == mkdir(existing.c_str(), dirmode))
                return false;
        } else {
            // Exists, check that it is a directory.
            struct stat info;
            err = fstat(fd, &info);
            close(fd);
            if (err == -1)
                return false;
            if ((info.st_mode & S_IFDIR) != S_IFDIR)
                return false;
        }
        existing.push_back(Separator);
    }
    return true;
}

static const size_t Divider = 36;
static const char Material[Divider + 1] =
    "0123456789abcdefghijklmnopqrstuvwxyz";
static const size_t Limit = 1296;

std::string FileOwner::make_name() {
    // One char file names are directories, file itself has two. No collision.
    std::string result;
    size_t left = index;
    do {
        result.push_back(Material[left % Divider]);
        left /= Divider;
        if (Limit <= left)
            result.push_back(Separator);
    } while (left > 0);
    if (result.size() < 2)
        result.push_back('0'); // Fill up. Generated name can not end in 0.
    return result;
}

bool FileOwner::append(RawData::ConstIterator From, RawData::ConstIterator To) {
    if (!present) {
        std::string name = make_name();
        if (!make_dirs(full, name))
            return false;
        full += name;
        errno = 0;
        fd = open(full.c_str(), O_WRONLY | O_CREAT | O_EXCL | O_CLOEXEC, mode);
        if (fd == -1)
            return false;
        present = true;
    }
    ssize_t written = write(fd, &(*From), To - From);
    if (written >= 0)
        length += written;
    return written != -1;
}

FileOwner::FileOwner(const std::string& Root, size_t Index)
    : owner(false), present(true), finished(true), in_error(false),
    index(Index), full(Root), fd(-1), length(0), mode(0)
{
    std::unique_lock<std::mutex> lock(FileOwner::index_mutex);
    assert(FileOwner::used.insert(index).second);
    lock.unlock();
    full += make_name();
    // File exists so we need to get the length.
    fd = open(full.c_str(), O_RDONLY | O_CLOEXEC);
    if (fd == -1) {
        in_error = true;
        return;
    }
    struct stat info;
    if (-1 == fstat(fd, &info)) {
        close(fd);
        fd = -1;
        in_error = true;
        return;
    }
    length = info.st_size;
}

FileOwner::FileOwner(const std::string& Root, mode_t FileMode)
    : owner(true), present(false), finished(false), in_error(false),
    index(0), full(Root), fd(-1), length(0), mode(FileMode)
{
    std::lock_guard<std::mutex> lock(FileOwner::index_mutex);
    if (FileOwner::freed.empty()) {
        index = FileOwner::used.size() + 1;
        if (FileOwner::used.insert(index).second)
            return;
        // Index is actually in use.
        // We have loaded existing catalog and freed set needs to be primed.
        size_t k = 1;
        for (auto iter : FileOwner::used) {
            for (; k < iter ; ++k)
                FileOwner::freed.insert(k);
            k = iter + 1; // Do not insert the used value.
            if (FileOwner::freed.size() > 100)
                break; // Let's say this is enough.
        }
    }
    auto iter = FileOwner::freed.cbegin();
    index = *iter;
    FileOwner::freed.erase(iter);
    assert(FileOwner::used.insert(index).second);
}

FileOwner::~FileOwner() {
    if (fd != -1)
        close(fd);
    if (owner && present)
        unlink(full.c_str());
    std::lock_guard<std::mutex> lock(FileOwner::index_mutex);
    assert(1 == FileOwner::used.erase(index));
    assert(FileOwner::freed.insert(index).second);
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

const std::string& FileOwner::FullName() const {
    assert(present);
    return full;
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
    return std::shared_ptr<DataReader>(new FileReader(full, Owner));
}
