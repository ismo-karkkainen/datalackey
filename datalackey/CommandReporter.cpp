//
//  CommandReporter.hpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 4.12.2018
//  Copyright © 2018 Ismo Kärkkäinen. All rights reserved.
//

#include "CommandReporter.hpp"
#include <memory>
#include <cassert>


CommandReporter::CommandReporter() { }

void CommandReporter::Register(const CommandDescription* D) {
    auto result = registered.insert(D);
    assert(result.second);
}

void CommandReporter::Report(OutputItem* Writer) const {
    for (auto iter : registered)
        iter->Report(Writer);
}

CommandReporter& CommandReporter::Get() {
    static CommandReporter reporter;
    return reporter;
}
