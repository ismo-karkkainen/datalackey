//
//  Process.hpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 23.10.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#ifndef Process_hpp
#define Process_hpp

#include "Encoder.hpp"
#include "ProcessInput.hpp"
#include <unistd.h>


// Base class for Process classes.
class Process {
public:
    virtual ~Process();

    // Returns a clone of the encoder in use, or nullptr if no input specified.
    virtual Encoder* Encoder() const = 0;

    virtual bool Run() = 0;
    virtual void Feed(std::vector<std::shared_ptr<ProcessInput>>& Inputs) = 0;
    virtual void EndFeed() = 0;
    virtual bool Closed() = 0;
    virtual bool Terminate() = 0;
    virtual bool Finished() const = 0;

    virtual pid_t PID() const = 0;
};


#endif /* Process_hpp */
