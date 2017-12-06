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
#include "SimpleValue.hpp"
#include <map>
#include <memory>
#include <stack>
#include <thread>


class LocalProcesses : public Processes {
private:
    Storage& storage;

    std::map<SimpleValue*, Process*> processes;
    mutable std::mutex processes_mutex;

    std::stack<SimpleValue*> to_delete;
    mutable std::mutex to_delete_mutex;

    bool terminate;
    void deleter();
    std::thread* cleaner;

public:
    LocalProcesses(Storage& S);
    ~LocalProcesses();

    bool Finished() const;

    std::vector<std::tuple<SimpleValue*,pid_t>> List() const;
    bool Terminate(const SimpleValue& Id);
    void Run(Output& Out,
        const SimpleValue& Id, std::vector<SimpleValue*>& Parameters);

    void HasFinished(const SimpleValue& Id);
};


#endif /* LocalProcesses_hpp */
