//
//  Structure.hpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 16.3.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//
// Licensed under Universal Permissive License. See License.txt.

#ifndef Structure_hpp
#define Structure_hpp


// For handling structured data within output item.
enum Structure {
    Array = 2247, // Each value is a ValueReference or array/dictionary.
    Dictionary, // Presumes key/value pairs in that order. One value each.
    End, // End of any of the previous.
    True,
    False,
    Null,
    RawItem // Marks that one RawData will be directly written.
};

#endif /* Structure_hpp */
