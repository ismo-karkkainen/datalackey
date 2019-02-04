//
//  Storage.cpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 14.3.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//
// Licensed under Universal Permissive License. See License.txt.

#include "Storage.hpp"
#include "OutputCollection.hpp"
#include "Messages.hpp"


Storage::~Storage() { }

void Storage::NotifyDelete(const std::string& Label, long long int Serial,
    Output* Out) const
{
    if (Out == nullptr)
        DataNotifiedOutputs.Notify([&Label, Serial](Output* Out) {
            ntf_data_deleted.Send(*Out, Label.c_str(), Serial); });
    else
        ntf_data_deleted.Send(*Out, Label.c_str(), Serial);
}

void Storage::NotifyStore(const std::string& Label, long long int Serial,
    Output* Out) const
{
    if (Out == nullptr)
        DataNotifiedOutputs.Notify([&Label, Serial](Output* Out) {
            ntf_data_stored.Send(*Out, Label.c_str(), Serial);
        });
    else
        ntf_data_stored.Send(*Out, Label.c_str(), Serial);
}

void Storage::NotifyError(const std::string& Label, long long int Serial,
    Output* Out) const
{
    if (Out == nullptr)
        DataNotifiedOutputs.Notify([&Label, Serial](Output* Out) {
            ntf_data_error.Send(*Out, Label.c_str(), Serial);
        });
    else
        ntf_data_error.Send(*Out, Label.c_str(), Serial);
}
