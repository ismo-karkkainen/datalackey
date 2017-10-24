//
//  Storage.cpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 14.3.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#include "Storage.hpp"


Storage::~Storage() {
}

void Storage::Store(const Label& L, const std::string& Format, RawData& Value) {
    Store(L, Format.c_str(), Value);
}

bool Preload(const Label& L, const std::string& Format) {
    return Preload(L, Format.c_str());
}

bool IsReady(const Label& L, const std::string& Format) {
    return IsReady(L, Format.c_str());
}

std::shared_ptr<const RawData> Data(const Label& L, const std::string& Format) {
    return Data(L, Format.c_str());
}

std::shared_ptr<const RawData> ReadyData(const Label& L,
    const std::string& Format)
{
    return ReadyData(L, Format.c_str());
}
