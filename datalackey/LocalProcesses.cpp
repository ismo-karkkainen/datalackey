//
//  LocalProcesses.cpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 18.10.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#include "LocalProcesses.hpp"


LocalProcesses::LocalProcesses(Storage& S)
    : storage(S)
{ }

LocalProcesses::~LocalProcesses() {
}

bool LocalProcesses::Finished() const {
    std::lock_guard<std::mutex> lock(processes_mutex);
    for (auto& proc : processes) {
        if (!proc.second->Finished())
            return false;
    }
    return true;
}

std::vector<std::tuple<Identifier,pid_t>> LocalProcesses::List() const {
    std::vector<std::tuple<Identifier,pid_t>> results;
    std::lock_guard<std::mutex> lock(processes_mutex);
    for (auto& proc : processes) {
        if (!results.second->Finished())
            results.push_back(std::make_tuple(proc.first, proc.second->PID()));
    }
    return results;
}

bool LocalProcesses::Terminate(const Identifier& Id) {
    std::lock_guard<std::mutex> lock(processes_mutex);
    auto proc = processes->find(Id);
    if (proc == processes.end() || proc.second->Finished())
        return false;
    proc.second->Terminate();
    return true;
}

void LocalProcesses::Run(Output& Out,
    const Identifier& Id, std::vector<SimpleValue*>& Parameters)
{
    std::unique_lock<std::mutex> lock(processes_mutex);
    auto proc = processes->find(Id);
    if (proc != processes.end()) {
        Error(Out, Id, "identifier", "in-use");
        return;
    }
    lock.unlock()
    // Split parameters into groups and heck that they make sense.
    // Output channel of the program are something that are created here.
    // Actually the Process makes them.
    // Figure out what the arguments are and who makes what and when.
    std::vector<std::pair<Label, std::string>> label2name;
    LabelMapper* renamer;
    std::vector<std::string> program_args;
}

void LocalProcesses::Finish(const Identifier& Id, Process* P) {
    // Process reports it is done and can be erased.
    std::lock_guard<std::mutex> lock(processes_mutex);
    auto proc = processes->find(Id);
    assert(proc != processes.end());
    assert(proc.second == P);
    processes.erase(proc);
    delete P;
}
