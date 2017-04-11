//
//  test_FileLister.cpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 23.3.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#include "FileLister.hpp"
#include "StdOutErr.hpp"
#include "Value_t.hpp"
#include "Literal.hpp"

int main(int argc, char** argv) {
    StdOutErr out;
    FileLister lister(argv[1], out);
    std::pair<std::string, std::string> path_name;
    while (bool(lister)) {
        lister >> path_name;
        out << Message << ValueRef<std::string>(path_name.first) << Literal(" ") <<
            ValueRef<std::string>(path_name.second) << End;
    }
    return 0;
}
