//
//  test_MessagePassThrough.cpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 17.7.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#include "JSONEncoder.hpp"
#include "FileDescriptorOutput.hpp"
#include "Output.hpp"
#include "FileDescriptorInput.hpp"
#include "MessagePassThrough.hpp"
#include "MemoryStorage.hpp"
#include "StorageDataSinkJSON.hpp"
#include "InputScannerJSON.hpp"
#include <cstring>
#include <ctime>
#include <iostream>

int main(int argc, char** argv) {
    JSONEncoder enc;
    FileDescriptorOutput out_channel(1);
    Output out(enc, out_channel);
    FileDescriptorInput in_channel;
    MessagePassThrough mpt(out);
    MemoryStorage storage(0, 0);
    StorageDataSinkJSON front(storage, out);
    InputScannerJSON scanner(in_channel, mpt, front, out);
    scanner.Scan();
    while (!scanner.Ended()) {
        struct timespec ts;
        ts.tv_sec = 0;
        ts.tv_nsec = 10000000;
        nanosleep(&ts, nullptr);
    }
    scanner.FinishScan();
    return 0;
}
