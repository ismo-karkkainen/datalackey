//
//  FileDescriptorInput.cpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 24.5.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//
// Licensed under Universal Permissive License. See License.txt.

#include "FileDescriptorInput.hpp"
#include <sys/select.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <cerrno>


static const int ReadBlockSize = 65536;


FileDescriptorInput::FileDescriptorInput(int FileDescriptor)
    : eof(false), fd(FileDescriptor)
{ }

FileDescriptorInput::~FileDescriptorInput() {
}

int FileDescriptorInput::Read(RawData& Buffer) {
    struct timespec ts;
    fd_set stdin;
    int total = 0;
    while (true) {
        ts.tv_sec = 0;
        ts.tv_nsec = 0;
        FD_ZERO(&stdin);
        FD_SET(fd, &stdin);
        errno = 0;
        int avail = pselect(fd + 1, &stdin, nullptr, nullptr, &ts, nullptr);
        if (avail <= 0) {
            eof = avail < 0 && errno == EBADF;
            return 0;
        }
        char* buf = Buffer.Get(ReadBlockSize);
        errno = 0;
        int got = read(fd, buf, ReadBlockSize);
        if (got <= 0) {
            eof = got == 0 || (got < 0 && !(errno == EAGAIN || errno == EINTR));
            got = 0;
        }
        total += got;
        if (got < ReadBlockSize) {
            Buffer.Discard(ReadBlockSize - got);
            break;
        }
    }
    return total;
}

bool FileDescriptorInput::Ended() {
    return eof;
}
