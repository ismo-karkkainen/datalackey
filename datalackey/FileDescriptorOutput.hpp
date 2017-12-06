//
//  FileDescriptorOutput.hpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 24.11.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

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

    OutputChannel& operator<<(const RawData& Buffer);
    void Write(RawData::ConstIterator& Start, RawData::ConstIterator& End);
    void Flush();
    bool Failed() const;
};


#endif /* FileDescriptorOutput_hpp */
