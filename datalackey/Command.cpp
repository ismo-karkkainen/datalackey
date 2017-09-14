//
//  Command.cpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 30.3.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#include "Command.hpp"

Command::Command(const char *const Name, Output& Out) : out(Out), name(Name) { }

Command::~Command() { }
