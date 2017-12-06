//
//  FileDescriptorInput.hpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 24.5.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#ifndef FileDescriptorInput_hpp
#define FileDescriptorInput_hpp

#include "InputChannel.hpp"
#include "RawData.hpp"


class FileDescriptorInput : public InputChannel {
private:
    bool eof;
    int fd;

public:
    FileDescriptorInput(int FileDescriptor = 0);
    ~FileDescriptorInput();
    int Read(RawData& Buffer);
    bool Ended();
};


#endif /* FileDescriptorInput_hpp */
