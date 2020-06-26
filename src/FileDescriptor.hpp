//
//  FileDescriptor.hpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 26.6.2020.
//  Copyright © 2020 Ismo Kärkkäinen. All rights reserved.
//
// Licensed under Universal Permissive License. See License.txt.

#ifndef FileDescriptor_hpp
#define FileDescriptor_hpp


class FileDescriptor {
private:
    int fd;

public:
    FileDescriptor(int FD = -1);
    FileDescriptor(const FileDescriptor&) = delete;
    FileDescriptor& operator=(const FileDescriptor&) = delete;
    ~FileDescriptor();

    void Close();
    inline bool Closed() const { return fd == -1; }
    inline int Descriptor() const { return fd; }
};

#endif
