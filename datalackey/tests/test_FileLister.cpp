//
//  test_FileLister.cpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 23.3.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#include "FileLister.hpp"
#include <iostream>

int main(int argc, char** argv) {
    FileLister lister(argv[1], std::cerr, std::cerr);
    std::pair<std::string, std::string> path_name;
    while (bool(lister)) {
        lister >> path_name;
        std::cout << path_name.first << " " << path_name.second << "\n";
    }
    return 0;
}
