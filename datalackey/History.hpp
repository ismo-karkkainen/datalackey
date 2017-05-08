//
//  History.hpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 14.3.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#ifndef History_hpp
#define History_hpp

#include <stdio.h>

class History {
    // File name for storage.
    // Save method.
    
public:
    History(); // Takes storage file name. Also storage object.
    ~History(); // Saves history.
    // Return history given desired outcomes. Full history if no outcome.
    // Some kind of way to restrict size? Prune old ones.
    // Add new command. With time-stamp?
};

#endif /* History_hpp */
