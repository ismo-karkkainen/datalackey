//
//  Factories.hpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 2.1.18.
//  Copyright © 2018 Ismo Kärkkäinen. All rights reserved.
//
// Licensed under Universal Permissive License. See License.txt.

#ifndef Factories_hpp
#define Factories_hpp

#include "Output.hpp"
#include "MessageHandler.hpp"
#include "SimpleValue.hpp"
#include "StringValueMapper.hpp"
#include "Storage.hpp"
#include "StorageDataSink.hpp"
#include "Processes.hpp"


MessageHandler* MakeMessageHandler(const char *const Format,
    Output& Out, Storage& S, Processes& P, SimpleValue* Identifier);
StorageDataSink* MakeStorageDataSink(const char *const Format,
    Storage& S, StringValueMapper* M, Output& Out, SimpleValue* Identifier);
InputScanner* MakeInputScanner(const char *const Format,
    InputChannel& IC, MessageHandler& MH, StorageDataSink& SDS,
    Output& Out, SimpleValue* Identifier);


#endif /* Factories_hpp */
