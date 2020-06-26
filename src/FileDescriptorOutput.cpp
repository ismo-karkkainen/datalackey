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

FileDescriptorOutput::FileDescriptorOutput(std::shared_ptr<FileDescriptor>& FD)
    : fd(FD), failed(false)
{ }

FileDescriptorOutput::~FileDescriptorOutput() { }

size_t FileDescriptorOutput::Write(
    RawData::ConstIterator& Start, RawData::ConstIterator& End)
{
    if (!failed && Start != End) {
        errno = 0;
        ssize_t result = write(fd->Descriptor(), &(*Start), End - Start);
        int err = errno;
        if (result == -1) {
            switch (err) {
            case EAGAIN:
                break; // No space, ok.
            case EBADF: // Fall through.
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
        fsync(fd->Descriptor());
}

void FileDescriptorOutput::Close() {
    fd->Close();
}

bool FileDescriptorOutput::Failed() const {
    return failed;
}

bool FileDescriptorOutput::Closed() const {
    return fd->Closed();
}
