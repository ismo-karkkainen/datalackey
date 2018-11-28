//
//  OutputCollection.hpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 16.3.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#ifndef OutputCollection_hpp
#define OutputCollection_hpp


#include "Output.hpp"
#include <vector>
#include <mutex>
#include <set>
#include <functional>


// Stores pointers to output objects that want to or can receive notifications.
class OutputCollection {
private:
    std::set<Output*> collection;
    std::mutex mutex;

    friend class Output;

    void Add(Output* O);
    void Remove(Output* O);

public:
    void Notify(Output* MessageOutput,
        std::function<void(Output*)> MessageToOutput,
        std::function<void(Output*)> NotificationToOthers);
    void Notify(Output* SkipOutput,
        std::function<void(Output*)> NotificationToOthers);
};

extern OutputCollection DataNotifiedOutputs;
extern OutputCollection ProcessNotifiedOutputs;


#endif /* OutputCollection_hpp */
