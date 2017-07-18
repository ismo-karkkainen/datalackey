//
//  StorageFrontJSON.hpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 26.5.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#ifndef StorageFrontJSON_hpp
#define StorageFrontJSON_hpp

#include "StorageFront.hpp"
#include "Storage.hpp"
#include <vector>


class StorageFrontJSON : public StorageFront {
private:
    Storage& storage;
    std::vector<char> key;
    std::vector<char> value;

public:
    StorageFrontJSON(Storage& S);
    ~StorageFrontJSON();
    const char *const Format() const;
    void Input(Iterator& Start, Iterator& End);
    void End();
    void Discard(Iterator& Start, Iterator& End);
};


#endif /* StorageFrontJSON_hpp */
