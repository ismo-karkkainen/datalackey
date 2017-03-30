//
//  FileLister.cpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 16.3.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#include "FileLister.hpp"

namespace bfs = boost::filesystem;

bool FileLister::process_directory() {
    if (directories.empty())
        return false;
    std::string root = directories.front();
    directories.pop();
    try {
        bfs::directory_iterator iter(root);
        bfs::directory_iterator end;
        while (iter != end) {
            bfs::directory_entry current = *iter;
            switch (current.status().type()) {
            case bfs::directory_file:
                directories.push(current.path().native());
                break;
            case bfs::regular_file:
                filenames.push(current.path());
                break;
            case bfs::symlink_file: {
                    bfs::path c = bfs::canonical(current.path());
                    if (bfs::is_directory(c))
                        directories.push(c.native());
                    else if (bfs::is_regular(c))
                        filenames.push(c);
                    else
                        out << Warning <<
                            "Unhandled file type: " << c.native() << "\n";
                }
                break;
            default:
                out << Warning <<
                    "Unhandled file type: " << current.path().native() << "\n";
                break;
            }
            iter++;
        }
    }
    catch (const bfs::filesystem_error& ex) {
        out << Error << ex.what();
        return false;
    }
    return true;
}

bool FileLister::get_more_files() {
    while (filenames.empty())
        if (!process_directory())
            return false;
    return true;
}

FileLister::FileLister(const std::string& root, MessageOutput& Out)
    : out(Out)
{
    if (bfs::exists(root) && bfs::is_directory(root)) {
        directories.push(root);
    } else {
        out << Error << "Not a directory: " << root << "\n";
    }
}

FileLister& FileLister::operator>>(std::pair<std::string, std::string>& path_name)
{
    if (filenames.empty() && !get_more_files())
        return *this;
    path_name.first = filenames.front().parent_path().native();
    path_name.second = filenames.front().filename().native();
    filenames.pop();
    return *this;
}

FileLister::operator bool() {
    if (!filenames.empty())
        return true;
    return get_more_files();
}
