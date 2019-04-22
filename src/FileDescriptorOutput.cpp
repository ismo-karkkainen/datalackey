//
//  FileDescriptorOutput.cpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 24.11.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//
// Licensed under Universal Permissive License. See License.txt.

#include "FileDescriptorOutput.hpp"
#include <unistd.h>
#include <cerrno>

FileDescriptorOutput::FileDescriptorOutput(int FileDescriptor)
    : failed(false), fd(FileDescriptor)
{ }

FileDescriptorOutput::~FileDescriptorOutput() { }

size_t FileDescriptorOutput::Write(
    RawData::ConstIterator& Start, RawData::ConstIterator& End)
{
    if (!failed && Start != End) {
        errno = 0;
        ssize_t result = write(fd, &(*Start), End - Start);
        int err = errno;
        if (result == -1) {
            switch (err) {
            case EAGAIN:
                break; // No space, ok.
            case EBADF:
                failed = true;
                fd = -1;
                break;
            default:
                failed = true;
                Close(); // Output is messed up now.
                break;
            }
            return 0;
        }
        return result;
    }
    return 0;
}

void FileDescriptorOutput::Flush() {
    if (!failed)
        fsync(fd);
}

void FileDescriptorOutput::Close() {
    // It was claimed we do not own fd but here we act as if we do.
    if (fd != -1)
        close(fd);
    fd = -1;
}

bool FileDescriptorOutput::Failed() const {
    return failed;
}

bool FileDescriptorOutput::Closed() const {
    return fd == -1;
}
