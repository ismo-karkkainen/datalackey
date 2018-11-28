//
//  OutputCollection.cpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 16.3.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#include "OutputCollection.hpp"
#include "Value_t.hpp"
#include "Structure.hpp"
#include <cassert>


OutputCollection DataNotifiedOutputs;
OutputCollection ProcessNotifiedOutputs;


void OutputCollection::Add(Output* O) {
    std::lock_guard<std::mutex> lock(mutex);
    collection.insert(O);
}

void OutputCollection::Remove(Output* O) {
    std::lock_guard<std::mutex> lock(mutex);
    collection.erase(O);
}

void OutputCollection::Notify(Output* MessageOutput,
    std::function<void(Output*)> MessageToOutput,
    std::function<void(Output*)> NotificationToOthers)
{
    std::lock_guard<std::mutex> lock(mutex);
    for (Output* out : collection) {
        if (out == MessageOutput)
            MessageToOutput(out);
        else
            NotificationToOthers(out);
    }
}

void OutputCollection::Notify(Output* SkipOutput,
    std::function<void(Output*)> NotificationToOthers)
{
    std::lock_guard<std::mutex> lock(mutex);
    for (Output* out : collection)
        if (out != SkipOutput)
            NotificationToOthers(out);
}
