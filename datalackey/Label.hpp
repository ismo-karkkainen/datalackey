//
//  Label.hpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 24.10.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#ifndef Label_hpp
#define Label_hpp

#include "SimpleValue.hpp"
#include <string>


class Label : public SimpleValue {
private:
    // JSON object keys are strings so this is in practically set in stone.
    std::string label;

public:
    Label(const std::string& Name);

    bool IsNumber() const;
    long long int Number() const;
    const std::string& String() const;

    inline operator std::string() const { return label; }

    inline bool operator<(const Label& L) const { return label < L.label; }
    inline bool operator!=(const Label& L) const { return label != L.label; }
};


#endif /* Label_hpp */
