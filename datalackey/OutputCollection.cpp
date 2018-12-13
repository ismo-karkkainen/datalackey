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


OutputCollection::OutputCollection() { }

void OutputCollection::Add(Output* O) {
    std::lock_guard<std::mutex> lock(mutex);
    collection.insert(O);
}

void OutputCollection::Remove(Output* O) {
    std::lock_guard<std::mutex> lock(mutex);
    collection.erase(O);
}

void OutputCollection::Notify(std::function<void(Output*)> Sender) {
    std::lock_guard<std::mutex> lock(mutex);
    for (Output* out : collection)
        Sender(out);
}

OutputCollection DataNotifiedOutputs;
OutputCollection ProcessNotifiedOutputs;
