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
#include <utility>
#include <string>
#include <unistd.h>


// Base class for Processes classes.
class Processes {
public:
    virtual ~Processes();

    virtual bool Finished() const = 0;

    // Return a list of identifier to process id (or similar) mappings.
    // Process ids are only for convenience reporting to user.
    virtual std::vector<std::pair<std::string,pid_t>> List() const = 0;

    // Terminate process. Return true if process exists.
    virtual bool Terminate(const SimpleValue& Id) = 0;

    // Start process. Ownership of parameters transfers.
    virtual void Run(Output& Out, const SimpleValue& Id,
        std::vector<std::shared_ptr<SimpleValue>>& Parameters) = 0;

    virtual void Feed(Output& Out, const SimpleValue& Id,
        std::vector<std::shared_ptr<SimpleValue>>& Parameters) = 0;

    virtual void EndFeed(Output& Out, const SimpleValue& Id,
        std::vector<std::shared_ptr<SimpleValue>>& Parameters) = 0;

    // Running process reports it has finished using this.
    virtual void HasFinished(const SimpleValue& Id) = 0;

    // Methods for sending notifications.
    void NotifyStart(const std::string& Id, pid_t PID,
        Output* Out = nullptr) const;
    void NotifyEnd(const std::string& Id, pid_t PID,
        Output* Out = nullptr) const;
};


#endif /* Processes_hpp */
