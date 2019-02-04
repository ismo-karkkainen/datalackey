//
//  FileDescriptorOutput.hpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 24.11.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//
// Licensed under Universal Permissive License. See License.txt.

#ifndef FileDescriptorOutput_hpp
#define FileDescriptorOutput_hpp

#include "OutputChannel.hpp"


class FileDescriptorOutput : public OutputChannel {
private:
    bool failed;
    int fd;

public:
    FileDescriptorOutput(int FileDescriptor);
    ~FileDescriptorOutput();

    size_t Write(RawData::ConstIterator& Start, RawData::ConstIterator& End);
    void Flush();
    void Close();
    bool Failed() const;
    bool Closed() const;
};


#endif /* FileDescriptorOutput_hpp */
