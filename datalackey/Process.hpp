//
//  Process.hpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 23.10.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#ifndef Process_hpp
#define Process_hpp

#include "Output.hpp"
#include "InputChannel.hpp"
#include "RawData.hpp"
#include "Identifier.hpp"
#include "Label.hpp"
#include "LabelMapper.hpp"
#include <vector>
#include <string>
#include <unistd.h>
#include <memory>
#include <utility>


// Base class for Process classes.
class Process {
protected:
    Output& out; // Status message output.
    Identifier id;
    std::vector<std::string> arguments;
    // For passing data to process.
    Storage& storage;
    std::vector<std::pair<Label, std::string>> label2name;
    Output& writer_provider; // May be accessed from outside, hence reference.
    // For passing results to storage and to caller.
    std::vector<InputChannel*> channels;
    LabelMapper* renamer;

public:
    // InputChannel* and StringMapper* ownerships transfer.
    Process(Output& StatusOut, Storage& S, const Identifier& Id,
        std::vector<std::string>& Arguments,
        std::vector<std::pair<Label, std::string>> Label2Name,
        Output& WriterProvider,
        std::vector<InputChannel*> ProgramOutputs, LabelMapper* Renamer);

    virtual ~Process();

    virtual bool Run() = 0;
    virtual bool Terminate() = 0;
    virtual bool Finished() const = 0;

    virtual pid_t PID() const = 0;
};


#endif /* Process_hpp */
