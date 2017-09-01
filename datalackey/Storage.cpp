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

void Preload(const std::string& Label, const std::string& Format) {
    Preload(Label, Format.c_str());
}

bool IsReady(const std::string& Label, const std::string& Format) {
    return IsReady(Label, Format.c_str());
}

std::shared_ptr<const RawData> Data(const std::string& Label,
    const std::string& Format)
{
    return Data(Label, Format.c_str());
}

