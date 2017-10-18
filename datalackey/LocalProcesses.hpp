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


class LocalProcesses : public Processes {
private:
    Storage& storage;

public:
    LocalProcesses(Storage& S);
    ~LocalProcesses();

    bool Finished() const;

    std::vector<std::tuple<std::string,pid_t>> List() const;
    bool Terminate(const std::string& Id);
    std::vector<std::string> Run(
        Output& Out, const std::vector<std::string>& Parameters);
};


#endif /* LocalProcesses_hpp */
