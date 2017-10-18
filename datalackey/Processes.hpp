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
    virtual std::vector<std::tuple<std::string,pid_t>> List() const = 0;
    // Terminate process. Return true if process exists.
    virtual bool Terminate(const std::string& Id) = 0;
    // Start process. Returns status message.
    virtual std::vector<std::string> Run(Output& Out,
        const std::vector<std::string>& Parameters) = 0;

    // Needs input and output channel information.
    // Needs arguments including program name.
    // Needs the data labels to feed to program.
    // Needs output label mapping and prefix/postfix for unexpected. Maybe else.
    // Arguments may need some way to get channel information.
    // Arguments could be list of strings and lists for key-chains to channels.
    // Channel information is naturally list of inputs and outputs, each item
    // in the list is a dictionary containing keys to data. Although the list
    // could be a dictionary since items are unique.
    // Technically we could deal with a string list and encode info but the
    // data needs to be mapped into some suitable internal representation.
    // Although if it is only shallow then creating objects and filling fields
    // is quite simple, but the identifier for channel needs to be specified
    // in a way that can be accessed. Or the caller passes the same data in
    // arguments as well as channel info. If needed, the string can be mapped
    // to a new value so it is simply a placeholder.
    // Depending on channel there could be fixed number of arguments and they
    // must be given or the command is erroneus. Just complain to user.
};


#endif /* Processes_hpp */
