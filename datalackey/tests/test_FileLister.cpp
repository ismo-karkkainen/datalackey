//
//  test_FileLister.cpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 23.3.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#include "FileLister.hpp"
#include "FileDescriptorOutput.hpp"
#include "Value_t.hpp"
#include "Literal.hpp"
#include "JSONEncoder.hpp"

int main(int argc, char** argv) {
    FileDescriptorOutput out(1);
    JSONEncoder encoder;
    Output results(encoder, out);
    OutputItem* writer = results.Writable();
    OutputItem* errors = results.Writable();
    FileLister lister(argv[1], *errors);
    std::pair<std::string, std::string> path_name;
    *writer << Array;
    while (bool(lister)) {
        lister >> path_name;
        std::string line = path_name.first + " " + path_name.second;
        *writer << ValueRef<std::string>(line);
    }
    *writer << End;
    delete writer;
    return 0;
}
