//
//  File.cpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 11.2.18.
//  Copyright © 2018 Ismo Kärkkäinen. All rights reserved.
//
// Licensed under Universal Permissive License. See License.txt.

#include "File.hpp"
#include "FileDescriptor.hpp"
#include <dirent.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/param.h>
#include <cerrno>
#include <memory>

#if defined(__linux__)
#include <limits.h>
#include <stdlib.h>
#endif


const char Separator = '/';

static std::string absolute(const std::string& Name, struct stat& info) {
    std::unique_ptr<FileDescriptor> fd(FileDescriptor::Open(Name.c_str()));
    if (fd == nullptr)
        return std::string();
    std::string abs;
    std::unique_ptr<char> buffer(new char[MAXPATHLEN + 1]);
#if defined(__APPLE__)
    if (-1 != fcntl(fd->Descriptor(), F_GETPATH, buffer.get()))
#elif defined(__linux__) || defined(__NetBSD__) || defined(__OpenBSD__) || defined(__DragonFly__) || defined(__FreeBSD__)
    if (buffer.get() == realpath(Name.c_str(), buffer.get()))
#endif
        abs = std::string(buffer.get());
    if (-1 == fstat(fd->Descriptor(), &info))
        return std::string();
    return abs;
}

std::string AbsoluteDirectory(const std::string& Name) {
    struct stat info;
    std::string abs = absolute(Name, info);
    if (abs.empty() || ((info.st_mode & S_IFDIR) != S_IFDIR))
        return std::string();
    return abs;
}

std::string AbsoluteFile(const std::string& Name, bool Executable) {
    struct stat info;
    std::string abs = absolute(Name, info);
    if (abs.empty() || ((info.st_mode & S_IFDIR) == S_IFDIR))
        return std::string();
    if (Executable) {
        if (info.st_uid == getpid() &&
            (info.st_mode & (S_IRUSR | S_IXUSR)) == (S_IRUSR | S_IXUSR))
                return abs;
        if (info.st_gid == getgid() &&
            (info.st_mode & (S_IRGRP | S_IXGRP)) == (S_IRGRP | S_IXGRP))
                return abs;
        if ((info.st_mode & (S_IROTH | S_IXOTH)) == (S_IROTH | S_IXOTH))
            return abs;
        return std::string();
    }
    return abs;
}
