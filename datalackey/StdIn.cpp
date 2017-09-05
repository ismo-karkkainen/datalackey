//
//  StdIn.cpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 24.5.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#include "StdIn.hpp"
#include <sys/select.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <cerrno>


StdIn::StdIn()
    : eof(false)
{ }

StdIn::~StdIn() {
}

int StdIn::Read(RawData& Buffer) {
    struct timespec ts;
    ts.tv_sec = 0;
    ts.tv_nsec = 0;
    fd_set stdin;
    FD_ZERO(&stdin);
    FD_SET(0, &stdin);
    errno = 0;
    int avail = pselect(1, &stdin, nullptr, nullptr, &ts, nullptr);
    if (avail <= 0) {
        eof = avail < 0 && errno == EBADF;
        return 0;
    }
    char* buf = Buffer.Get(avail);
    errno = 0;
    int total = read(0, buf, avail);
    if (total <= 0) {
        eof = total == 0 || total < 0 && !(errno == EAGAIN || errno == EINTR);
        total = 0;
    }
    if (total < avail)
        Buffer.Discard(avail - total);
    return total;
}

bool StdIn::Ended() {
    return eof;
}
