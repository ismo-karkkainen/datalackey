//
//  DataGroup.hpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 9.1.18.
//  Copyright © 2018 Ismo Kärkkäinen. All rights reserved.
//

#ifndef DataGroup_hpp
#define DataGroup_hpp

#include "RawData.hpp"
#include "StringValue.hpp"
#include "DataOwner.hpp"
#include <map>
#include <memory>
#include <string>
#include <vector>


class DataGroup {
private:
    std::map<StringValue,std::shared_ptr<DataOwner>> label2data;
    std::shared_ptr<DataOwner> incoming;
    std::string format;

    static DataOwnerGenerator* dog;

public:
    static void SetDataOwnerGenerator(DataOwnerGenerator& G);

    DataGroup(const char *const Format);

    bool StartItem(const StringValue& Label);
    bool Append(RawData::ConstIterator& Start, RawData::ConstIterator& End);
    bool Finish();

    const std::string& Format() const { return format; }
    std::pair<StringValue,std::shared_ptr<DataOwner>> Get();

    size_t Size() const { return label2data.size(); }
};


#endif /* DataGroup_hpp */
