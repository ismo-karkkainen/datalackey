//
//  test_MessagePassThrough.cpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 17.7.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#include "JSONEncoder.hpp"
#include "StdOut.hpp"
#include "Output.hpp"
#include "StdIn.hpp"
#include "MessagePassThrough.hpp"
#include "MemoryStorage.hpp"
#include "StorageDataSinkJSON.hpp"
#include "InputScannerJSON.hpp"
#include <cstring>
#include <ctime>

int main(int argc, char** argv) {
    JSONEncoder enc;
    StdOut out_channel;
    Output out(enc, out_channel);
    StdIn in_channel;
    MessagePassThrough mpt(out);
    MemoryStorage storage;
    StorageDataSinkJSON front(storage);
    InputScannerJSON scanner(in_channel, mpt, front);
    scanner.Scan();
    while (!scanner.Ended()) {
        struct timespec ts;
        ts.tv_sec = 0;
        ts.tv_nsec = 10000000;
        nanosleep(&ts, nullptr);
    }
    return 0;
}
