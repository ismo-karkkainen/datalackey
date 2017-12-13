//
//  Processes.hpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 17.10.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#ifndef Processes_hpp
#define Processes_hpp

#include "Output.hpp"
#include "SimpleValue.hpp"
#include <vector>
#include <tuple>
#include <string>
#include <unistd.h>


// Base class for Processes classes.
class Processes {
public:
    virtual ~Processes();

    virtual bool Finished() const = 0;

    // Return a list of identifier to process id (or similar) mappings.
    // Process ids are only for convenence reporting to user.
    virtual std::vector<std::tuple<SimpleValue*,pid_t>> List() const = 0;

    // Terminate process. Return true if process exists.
    virtual bool Terminate(const SimpleValue& Id) = 0;

    // Wait for process to finish. Return true if process exists.
    virtual bool Wait(const SimpleValue& Id) = 0;

    // Start process. Ownership of parameters transfers.
    virtual void Run(Output& Out,
        const SimpleValue& Id, std::vector<SimpleValue*>& Parameters) = 0;

    // Running process reports it has finished using this.
    virtual void HasFinished(const SimpleValue& Id) = 0;
};


#endif /* Processes_hpp */
