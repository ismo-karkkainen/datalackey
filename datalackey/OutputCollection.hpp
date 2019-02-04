//
//  OutputCollection.hpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 16.3.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//
// Licensed under Universal Permissive License. See License.txt.

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

    OutputCollection(const OutputCollection&) = delete;
    OutputCollection& operator=(const OutputCollection&) = delete;

public:
    OutputCollection();
    void Notify(std::function<void(Output*)> Sender);
};

extern OutputCollection DataNotifiedOutputs;
extern OutputCollection ProcessNotifiedOutputs;


#endif /* OutputCollection_hpp */
