//
//  Time.cpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 29.11.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#include "Time.hpp"
#include <ctime>
#include <cassert>


void Nap(long Nanoseconds) {
    assert(0 <= Nanoseconds);
    struct timespec ts;
    ts.tv_sec = Nanoseconds / 1000000000;
    ts.tv_nsec = Nanoseconds % 1000000000;
    nanosleep(&ts, nullptr);
}
