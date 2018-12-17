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
#include "ProcessInput.hpp"
#include "Messages.hpp"
#include <map>
#include <memory>
#include <stack>
#include <thread>
#include <vector>
#include <utility>


class LocalProcesses : public Processes {
private:
    Storage& storage;

    std::map<SimpleValue*, Process*> processes;
    mutable std::mutex processes_mutex;

    std::stack<SimpleValue*> to_delete;
    mutable std::mutex to_delete_mutex;

    std::pair<bool,std::vector<std::shared_ptr<ProcessInput>>> feed(
        Output& Out, const SimpleValue& Id,
        std::vector<std::shared_ptr<SimpleValue>>& Parameters, Encoder* E,
        bool FromFeed = false);

public:
    LocalProcesses(Storage& S);
    ~LocalProcesses();

    bool CleanFinished();
    bool Finished() const;

    std::vector<std::pair<std::unique_ptr<SimpleValue>,pid_t>> List() const;
    bool Terminate(const SimpleValue& Id);
    void Run(Output& Out, const SimpleValue& Id,
        std::vector<std::shared_ptr<SimpleValue>>& Parameters);
    void Feed(Output& Out, const SimpleValue& Id,
        std::vector<std::shared_ptr<SimpleValue>>& Parameters);
    void EndFeed(Output& Out, const SimpleValue& Id,
        std::vector<std::shared_ptr<SimpleValue>>& Parameters);

    void HasFinished(const SimpleValue& Id);
};


#endif /* LocalProcesses_hpp */
