//
//  LocalProcess.hpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 31.10.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#ifndef LocalProcess_hpp
#define LocalProcess_hpp

#include "Process.hpp"
#include <vector>
#include <string>
#include <unistd.h>
#include <memory>
#include <utility>


// Base class for LocalProcess classes.
class LocalProcess {
protected:
    Output& out; // Status message output. Also pass-through to caller?
    Identifier id;
    std::vector<std::string> arguments;
    // For passing data to process.
    Storage& storage;
    std::vector<std::pair<Label, std::string>> label2name;
    // For passing results to storage and to caller.
    std::vector<InputChannel*> incoming_channels;
    std::vector<OutputChannel*> outgoing_channels;
    Output program_feed; // Provides output to program.
    LabelMapper* renamer;

public:
    // LabelMapper* ownership transfers.
    LocalProcess(Output& StatusOut, Storage& S, const Identifier& Id,
        std::vector<std::string>& Arguments,
        std::vector<std::tuple<std::string,std::string>> Environment,
        std::vector<std::pair<Label, std::string>> Label2Name,
        std::vector<std::vector<std::string>> ProgramInputInfo,
        std::vector<std::vector<std::string>> ProgramOutputInfo,
        LabelMapper* Renamer);
    // Arguments should have program output channel and what to do with the
    // information that is passed through. Raw, unformatted output wrapped?
    // If wrapped then it could be sent to status output.

    ~LocalProcess();

    bool Run();
    bool Terminate();
    bool Finished() const;

    pid_t PID() const;
};


#endif /* LocalProcess_hpp */
