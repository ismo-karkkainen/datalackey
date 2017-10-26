//
//  LocalProcesses.hpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 18.10.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#ifndef LocalProcesses_hpp
#define LocalProcesses_hpp

#include "Processes.hpp"
#include "Storage.hpp"
#include "Process.hpp"
#include <map>
#include <memory>


class LocalProcesses : public Processes {
private:
    Storage& storage;

    std::map<Identifier, Process*> processes;
    std::mutex processes_mutex;

public:
    LocalProcesses(Storage& S);
    ~LocalProcesses();

    bool Finished() const;

    std::vector<std::tuple<Identifier,pid_t>> List() const;
    bool Terminate(const Identifier& Id);
    void Run(Output& Out,
        const Identifier& Id, std::vector<SimpleValue*>& Parameters);
};


#endif /* LocalProcesses_hpp */
