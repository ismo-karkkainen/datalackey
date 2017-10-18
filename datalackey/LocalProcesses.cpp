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
    // Go through all processes and return false if any is running or has
    // unfinished channels.
    return true;
}

std::vector<std::tuple<std::string,pid_t>> LocalProcesses::List() const {
    // For each process, return identifier and pid pair.
    std::vector<std::tuple<std::string,pid_t>> results;
    return results;
}

bool LocalProcesses::Terminate(const std::string& Id) {
    // If process does not exist, return false.
    // Ask process owner to terminate it.
    return true;
}

std::vector<std::string> LocalProcesses::Run(
    Output& Out, const std::vector<std::string>& Parameters)
{
    std::vector<std::string> result;
    // If there is process by same name, return error.
    result.push_back(std::string("launching"));
    return result;
}
