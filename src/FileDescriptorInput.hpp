//
//  FileDescriptorInput.hpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 24.5.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//
// Licensed under Universal Permissive License. See License.txt.

#ifndef FileDescriptorInput_hpp
#define FileDescriptorInput_hpp

#include "InputChannel.hpp"
#include "RawData.hpp"
#include "FileDescriptor.hpp"
#include <memory>


class FileDescriptorInput : public InputChannel {
private:
    std::shared_ptr<FileDescriptor> fd;

public:
    FileDescriptorInput(std::shared_ptr<FileDescriptor>& FD);
    ~FileDescriptorInput();
    int Read(RawData& Buffer);
    bool Ended();
};


#endif /* FileDescriptorInput_hpp */
