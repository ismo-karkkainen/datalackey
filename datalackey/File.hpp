//
//  File.hpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 11.2.18.
//  Copyright © 2018 Ismo Kärkkäinen. All rights reserved.
//
// Licensed under Universal Permissive License. See License.txt.

#ifndef File_hpp
#define File_hpp

#include <string>


extern const char Separator;

std::string AbsoluteDirectory(const std::string& Name);
std::string AbsoluteFile(const std::string& Name, bool Executable = false);

#endif /* File_hpp */
