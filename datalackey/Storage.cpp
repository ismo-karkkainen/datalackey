//
//  Storage.cpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 14.3.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#include "Storage.hpp"


Storage::Storage() {
}

Storage::~Storage() {
}

void Storage::Store(const std::string& Label, const std::string& Format,
    RawData& Value)
{
    Store(Label, Format.c_str(), Value);
}

bool Preload(const std::string& Label, const std::string& Format) {
    return Preload(Label, Format.c_str());
}

bool IsReady(const std::string& Label, const std::string& Format) {
    return IsReady(Label, Format.c_str());
}

std::shared_ptr<const RawData> Data(const std::string& Label,
    const std::string& Format)
{
    return Data(Label, Format.c_str());
}

std::shared_ptr<const RawData> ReadyData(const std::string& Label,
    const std::string& Format)
{
    return ReadyData(Label, Format.c_str());
}
