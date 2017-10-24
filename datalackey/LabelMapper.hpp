//
//  LabelMapper.hpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 23.10.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#ifndef LabelMapper_hpp
#define LabelMapper_hpp

#include "Label.hpp"


// Base class for LabelMapper classes.
class LabelMapper {
public:
    virtual ~LabelMapper();

    virtual Label operator[](const Label& Value) const = 0;

    // All set-up can be done using the derived class methods.
};


#endif /* LabelMapper_hpp */
