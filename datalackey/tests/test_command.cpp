//
//  test_MessagePassThrough.cpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 17.7.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#include "JSONEncoder.hpp"
#include "StdOut.hpp"
#include "StdErr.hpp"
#include "Output.hpp"
#include "StdIn.hpp"
#include "CommandHandlerJSON.hpp"
#include "ListCommand.hpp"
#include "GetCommand.hpp"
#include "DeleteCommand.hpp"
#include "VersionCommand.hpp"
#include "MemoryStorage.hpp"
#include "StorageDataSinkJSON.hpp"
#include "InputScannerJSON.hpp"
#include "Options.hpp"
#include <cstring>
#include <ctime>
#include <limits>
#include <iostream>
#include <cassert>

static const char* inputs[1] = { "stdin" };
static const char* outputs[2] = { "stdout", "stderr" };
static const char* formats[1] = { "JSON" };


static int HandleArguments(int argc, char** argv) {
    opt::Add('i', "command-in");
    opt::AddValue(0, 1, inputs);
    opt::Usage("Command input channel.");
    opt::AddValue(0, 1, formats);
    opt::Usage("Channel data format.");
    opt::Add('o', "command-out");
    opt::AddValue(0, 2, outputs);
    opt::Usage("Command result output channel.");
    opt::AddValue(0, 1, formats);
    opt::Usage("Channel data format.");
    opt::Add('m', "memory");
    opt::Usage("Store data in memory. Give at least one of the following.");
    opt::AddValue(0, 0, std::numeric_limits<int>::max());
    opt::Usage("Megabytes of memory always allowed to use. 0 for no limit.");
    opt::AddValue(1024, 0, std::numeric_limits<int>::max());
    opt::Usage(
        "Megabytes of total memory attempted to keep free. 0 for no limit.");
    opt::AddFlag('h', "help", false);
    opt::Usage("Print help and exit.");
    opt::Doc("Commands:");
    opt::Doc("[ identifier, \"list\" ] to list data labels.");
    opt::Doc("[ identifier, \"get\", label1, label2, ... ] to get data.");
    opt::Doc("[ identifier, \"delete\", label1, label2, ... ] to delete data.");
    opt::Doc("[ identifier, \"version\" ] to return version.");
    if (opt::GivenOption('h', "help", argc, argv)) {
        opt::PrintUsage(std::cerr, "datalackey");
        return 0;
    }
    int rv = opt::ParseCommandLine(argc, argv, std::cerr);
    return rv ? rv : -1;
}

int main(int argc, char** argv) {
    int rv = HandleArguments(argc, argv);
    if (rv >= 0)
        return rv;

    Storage* storage = nullptr;
    if (opt::Given("memory"))
        storage = new MemoryStorage(static_cast<size_t>(opt::Int("memory", 1)),
            static_cast<size_t>(opt::Int("memory", 2)));
    assert(storage != nullptr);

    OutputChannel* out_channel = nullptr;
    std::string choice = opt::String("command-out", 1);
    if (choice == "stdout")
        out_channel = new StdOut();
    else if (choice == "stderr")
        out_channel = new StdErr();
    assert(out_channel != nullptr);

    Encoder* enc = nullptr;
    choice = opt::String("command-out", 2);
    if (choice == "JSON")
        enc = new JSONEncoder();
    assert(enc != nullptr);

    Output* out = new Output(*enc, *out_channel);
    choice = opt::String("command-in", 1);
    InputChannel* in_channel = nullptr;
    if (choice == "stdin")
        in_channel = new StdIn();
    assert(in_channel != nullptr);

    StorageDataSink* sink = nullptr;
    CommandHandler* command_handler = nullptr;
    InputScanner* scanner = nullptr;
    choice = opt::String("command-in", 2);
    if (choice == "JSON") {
        sink = new StorageDataSinkJSON(*storage, *out);
        command_handler = new CommandHandlerJSON(*out);
        scanner =
            new InputScannerJSON(*in_channel, *command_handler, *sink, *out);
    }
    assert(sink != nullptr);
    assert(command_handler != nullptr);
    assert(scanner != nullptr);

    // Add commands.
    ListCommand list("list", *out, *storage);
    command_handler->AddCommand(&list);
    GetCommand get("get", *out, *storage, enc->Format());
    command_handler->AddCommand(&get);
    DeleteCommand del("delete", *out, *storage);
    command_handler->AddCommand(&del);
    VersionCommand version("version", *out);
    command_handler->AddCommand(&version);

    scanner->Scan();
    while (!scanner->Ended()) {
        struct timespec ts;
        ts.tv_sec = 0;
        ts.tv_nsec = 10000000;
        nanosleep(&ts, nullptr);
    }
    scanner->FinishScan();

    delete scanner;
    delete command_handler;
    delete sink;
    delete in_channel;
    delete out;
    delete enc;
    delete out_channel;
    delete storage;
    return 0;
}
