//
//  CommandReporter.hpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 4.12.2018
//  Copyright © 2018 Ismo Kärkkäinen. All rights reserved.
//
// Licensed under Universal Permissive License. See License.txt.

#include "CommandReporter.hpp"
#include "Value_t.hpp"
#include <memory>


CommandReporter::CommandReporter() : registering(true) { }

void CommandReporter::Register(const CommandDescription* D) {
    if (registering)
        registered.push_back(D);
}

void CommandReporter::Report(OutputItem* Writer) const {
    for (auto iter : registered) {
        *Writer << ValueRef<std::string>(iter->Name());
        iter->Report(Writer);
    }
}

CommandReporter& CommandReporter::Get() {
    static CommandReporter reporter;
    return reporter;
}
