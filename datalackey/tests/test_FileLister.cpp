//
//  test_FileLister.cpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 23.3.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#include "FileLister.hpp"
#include "StdOutErr.hpp"

int main(int argc, char** argv) {
    StdOutErr out;
    FileLister lister(argv[1], out);
    std::pair<std::string, std::string> path_name;
    while (bool(lister)) {
        lister >> path_name;
        out << Normal << path_name.first << " " << path_name.second << "\n";
    }
    return 0;
}
