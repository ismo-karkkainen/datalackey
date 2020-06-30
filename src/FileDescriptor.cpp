//
//  FileDescriptor.cpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 26.6.2020.
//  Copyright © 2020 Ismo Kärkkäinen. All rights reserved.
//
// Licensed under Universal Permissive License. See License.txt.

#include "FileDescriptor.hpp"
#include <mutex>
#include <map>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>


static std::map<int,FileDescriptor*> owners;
static std::mutex owners_mutex;


FileDescriptor::FileDescriptor(int FD, bool) : fd(FD) {
    if (fd != -1)
        owners[fd] = this;
}

FileDescriptor::FileDescriptor(int FD) : fd(FD) {
    if (fd == -1)
        return;
    std::lock_guard<std::mutex> lock(owners_mutex);
    owners[fd] = this;
}

FileDescriptor::~FileDescriptor() {
    Close();
}
    
void FileDescriptor::Close() {
    if (fd == -1)
        return;
    std::lock_guard<std::mutex> lock(owners_mutex);
    decltype(owners)::iterator iter = owners.find(fd);
    if (iter->second == this) {
        //std::cerr << fd << " closed\n";
        close(fd);
        owners.erase(iter);
    }
    fd = -1;
}

bool FileDescriptor::Pipe(std::shared_ptr<FileDescriptor>& Read,
    std::shared_ptr<FileDescriptor>& Write)
{
    int p[2];
    std::lock_guard<std::mutex> lock(owners_mutex);
    errno = 0;
    if (-1 == pipe(p))
        return false;
    Read.reset(new FileDescriptor(p[0], false));
    Write.reset(new FileDescriptor(p[1], false));
    return true;
}

FileDescriptor* FileDescriptor::Open(const char* Filename, int Flags, mode_t Mode)
{
    std::lock_guard<std::mutex> lock(owners_mutex);
    errno = 0;
    int fd = open(Filename, Flags | O_CLOEXEC, Mode);
    if (fd == -1)
        return nullptr;
    return new FileDescriptor(fd, false);
}
