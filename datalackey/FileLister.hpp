//
//  FileLister.hpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 16.3.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#ifndef FileLister_hpp
#define FileLister_hpp

#include <string>
#include <queue>
#include "Output.hpp"
#include <utility>
#include <boost/filesystem.hpp>

class FileLister {
private:
    std::queue<std::string> directories;
    std::queue<boost::filesystem::path> filenames;
    bool process_directory();
    bool get_more_files();

    Output& out;
public:
    FileLister(const std::string& root, Output& Out);
    // Could offer path and file name for convenience?
    FileLister& operator>>(std::pair<std::string, std::string>& path_name);
    operator bool();
};

#endif /* FileLister_hpp */
