//
//  FileDescriptorInput.cpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 24.5.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//
// Licensed under Universal Permissive License. See License.txt.

#include "FileDescriptorInput.hpp"
#include <poll.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <cerrno>


static const int ReadBlockSize = 65536;


FileDescriptorInput::FileDescriptorInput(std::shared_ptr<FileDescriptor>& FD)
    : fd(FD)
{ }

FileDescriptorInput::~FileDescriptorInput() { }

int FileDescriptorInput::Read(RawData& Buffer) {
    int total = 0;
    while (true) {
        struct pollfd pfd;
        pfd.fd = fd->Descriptor();
        pfd.events = POLLIN;
        pfd.revents = 0;
        errno = 0;
        int avail = poll(&pfd, 1, 0);
        if (avail <= 0) {
            if (avail < 0 && (errno == EFAULT || errno == EINVAL))
                fd->Close();
            return 0;
        }
        char* buf = Buffer.Get(ReadBlockSize);
        errno = 0;
        int got = read(fd->Descriptor(), buf, ReadBlockSize);
        if (got <= 0) {
            if (got == 0 || (got < 0 && !(errno == EAGAIN || errno == EINTR)))
                fd->Close();
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
    return fd->Closed();
}
