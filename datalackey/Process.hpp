//
//  Process.hpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 23.10.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#ifndef Process_hpp
#define Process_hpp

#include <unistd.h>


// Base class for Process classes.
class Process {
public:
    virtual ~Process();

    virtual bool Run() = 0;
    virtual bool Terminate() = 0;
    virtual bool Finished() const = 0;

    virtual pid_t PID() const = 0;
};


#endif /* Process_hpp */
