//
//  Factories.cpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 2.1.18.
//  Copyright © 2018 Ismo Kärkkäinen. All rights reserved.
//

#include "Factories.hpp"
#include "GetCommand.hpp"
#include "DeleteCommand.hpp"
#include "RenameCommand.hpp"
#include "VersionCommand.hpp"
#include "StorageInfoCommand.hpp"
#include "RunCommand.hpp"
#include "FeedCommand.hpp"
#include "EndFeedCommand.hpp"
#include "TerminateCommand.hpp"
#include "NoOperationCommand.hpp"
#include "CommandHandlerJSON.hpp"
#include "MessageRawJSON.hpp"
#include "StorageDataSinkJSON.hpp"
#include "InputScannerJSON.hpp"
#include "InputScannerRawMessage.hpp"
#include "InputScannerDiscard.hpp"

#include <cstring>


MessageHandler* MakeMessageHandler(const char *const Format, 
    Output& Out, Storage& S, Processes& P, SimpleValue* Identifier)
{
    if (Format == nullptr || !strcmp(Format, "JSON")) {
        CommandHandler* ch = new CommandHandlerJSON(Out);
        ch->AddCommand(new StorageInfoCommand("storage-info", Out, S));
        ch->AddCommand(new GetCommand("get", Out, S));
        ch->AddCommand(new DeleteCommand("delete", Out, S));
        ch->AddCommand(new RenameCommand("rename", Out, S));
        ch->AddCommand(new NoOperationCommand("no-op", Out));
        ch->AddCommand(new VersionCommand("version", Out));
        ch->AddCommand(new RunCommand("run", Out, P));
        ch->AddCommand(new FeedCommand("feed", Out, P));
        ch->AddCommand(new EndFeedCommand("end-feed", Out, P));
        ch->AddCommand(new TerminateCommand("terminate", Out, P));
        return ch;
    }
    if (!strcmp(Format, "bytes")) {
        return new MessageRawJSON(Out, *Identifier);
    }
    return nullptr;
}

StorageDataSink* MakeStorageDataSink(const char *const Format, 
    Storage& S, StringValueMapper* M, Output& Out, SimpleValue* Identifier)
{
    if (Format == nullptr || !strcmp(Format, "JSON")) {
        return new StorageDataSinkJSON(S, Identifier, Out, M);
    }
    if (!strcmp(Format, "bytes")) {
        // If raw output storage as data is ever made, change this.
        return new StorageDataSinkJSON(S, nullptr, Out);
    }
    return nullptr;
}

InputScanner* MakeInputScanner(const char *const Format, 
    InputChannel& IC, MessageHandler& MH, StorageDataSink& SDS,
    Output& Out, SimpleValue* Identifier)
{
    if (Format == nullptr) {
        return new InputScannerDiscard(IC, MH, SDS);
    }
    if (!strcmp(Format, "JSON")) {
        return new InputScannerJSON(IC, MH, SDS, Out, Identifier);
    }
    if (!strcmp(Format, "bytes")) {
        return new InputScannerRawMessage(IC, MH, SDS);
    }
    return nullptr;
}
