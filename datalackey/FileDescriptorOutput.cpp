//
//  FileDescriptorOutput.cpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 24.11.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#include "FileDescriptorOutput.hpp"
#include <unistd.h>

FileDescriptorOutput::FileDescriptorOutput(int FileDescriptor)
    : failed(false), fd(FileDescriptor)
{ }

FileDescriptorOutput::~FileDescriptorOutput() { }

OutputChannel& FileDescriptorOutput::operator<<(const RawData& Buffer) {
    if (!failed) {
        if (-1 == write(fd, Buffer.Raw(), Buffer.Size()))
            failed = true;
    }
    return *this;
}

void FileDescriptorOutput::Write(
    RawData::ConstIterator& Start, RawData::ConstIterator& End)
{
    if (!failed) {
        if (-1 == write(fd, &(*Start), End - Start))
            failed = true;
    }
}

void FileDescriptorOutput::Flush() {
    if (!failed)
        fsync(fd);
}

bool FileDescriptorOutput::Failed() const {
    return failed;
}
