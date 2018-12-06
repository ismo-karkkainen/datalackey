//
//  CommandReporter.hpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 4.12.2018
//  Copyright © 2018 Ismo Kärkkäinen. All rights reserved.
//

#ifndef CommandReporter_hpp
#define CommandReporter_hpp


#include "CommandDescription.hpp"
#include "OutputItem.hpp"
#include <set>


// Used for asking a report of all messages that can be sent.
class CommandReporter {
private:
    std::set<const CommandDescription*> registered;

    CommandReporter();

public:
    void Register(const CommandDescription* D);
    void Report(OutputItem* Writer) const;

    static CommandReporter& Get();
};


#endif /* CommandReporter_hpp */
