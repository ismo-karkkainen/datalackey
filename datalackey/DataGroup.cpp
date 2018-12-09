//
//  DataGroup.cpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 9.1.18.
//  Copyright © 2018 Ismo Kärkkäinen. All rights reserved.
//

#include "DataGroup.hpp"
#include <cassert>


DataOwnerGenerator* DataGroup::dog = nullptr;


void DataGroup::SetDataOwnerGenerator(DataOwnerGenerator& G) {
    assert(DataGroup::dog == nullptr);
    DataGroup::dog = &G;
}

DataGroup::DataGroup(const char *const Format)
    : format(Format)
{
    assert(DataGroup::dog != nullptr);
}

bool DataGroup::StartItem(const StringValue& Label) {
    Finish();
    auto iter = label2data.find(Label);
    if (iter != label2data.end())
        return false;
    label2data[Label] = incoming = std::shared_ptr<DataOwner>(dog->Generate());
    return true;
}

bool DataGroup::Append(
    RawData::ConstIterator& Start, RawData::ConstIterator& End)
{
    if (!incoming)
        return false;
    return incoming->Append(Start, End);
}

bool DataGroup::Finish() {
    if (incoming) {
        incoming->Finish();
        incoming = nullptr;
    }
    return true;
}

std::pair<StringValue,std::shared_ptr<DataOwner>> DataGroup::Get() {
    if (label2data.empty())
        return std::pair<StringValue,std::shared_ptr<DataOwner>>(
            StringValue(), std::shared_ptr<DataOwner>());
    auto first = label2data.begin();
    std::pair<StringValue,std::shared_ptr<DataOwner>> val(
        first->first, first->second);
    label2data.erase(first);
    return val;
}
