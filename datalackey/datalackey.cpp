//
//  datalackey.cpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 7.12.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#include "JSONEncoder.hpp"
#include "FileDescriptorOutput.hpp"
#include "Output.hpp"
#include "FileDescriptorInput.hpp"
#include "CommandHandlerJSON.hpp"
#include "ListCommand.hpp"
#include "GetCommand.hpp"
#include "DeleteCommand.hpp"
#include "VersionCommand.hpp"
#include "LocalProcesses.hpp"
#include "ProcessesCommand.hpp"
#include "RunCommand.hpp"
#include "FeedCommand.hpp"
#include "TerminateCommand.hpp"
#include "NoOperationCommand.hpp"
#include "MemoryStorage.hpp"
#include "StorageDataSinkJSON.hpp"
#include "InputScannerJSON.hpp"
#include "Options.hpp"
#include "Time.hpp"
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
    opt::AddFlag('m', "memory", false);
    opt::Usage("Store data in memory.");
    opt::Add('f', "free");
    opt::AddValue(1024, 0, std::numeric_limits<int>::max());
    opt::Usage(
        "Megabytes of total memory attempted to keep free. 0 for no limit.");
    opt::AddFlag('h', "help", false);
    opt::Usage("Print help and exit.");
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
    if (opt::Bool("memory", 0)) {
        MemoryStorage* ms = new MemoryStorage();
        DataGroup::SetDataOwnerGenerator(*ms);
        storage = ms;
    }
    assert(storage != nullptr);

    OutputChannel* out_channel = nullptr;
    std::string choice = opt::String("command-out", 1);
    if (choice == "stdout")
        out_channel = new FileDescriptorOutput(1);
    else if (choice == "stderr")
        out_channel = new FileDescriptorOutput(2);
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
        in_channel = new FileDescriptorInput();
    assert(in_channel != nullptr);

    StorageDataSink* sink = nullptr;
    CommandHandler* command_handler = nullptr;
    InputScanner* scanner = nullptr;
    choice = opt::String("command-in", 2);
    if (choice == "JSON") {
        sink = new StorageDataSinkJSON(*storage, nullptr, *out);
        command_handler = new CommandHandlerJSON(*out);
        scanner =
            new InputScannerJSON(*in_channel, *command_handler, *sink, *out);
    }
    assert(sink != nullptr);
    assert(command_handler != nullptr);
    assert(scanner != nullptr);

    LocalProcesses* procs = new LocalProcesses(*storage);

    // Add commands.
    ListCommand list("list", *out, *storage);
    command_handler->AddCommand(&list);
    GetCommand get("get", *out, *storage, enc->Format());
    command_handler->AddCommand(&get);
    DeleteCommand del("delete", *out, *storage);
    command_handler->AddCommand(&del);
    VersionCommand version("version", *out);
    command_handler->AddCommand(&version);
    ProcessesCommand processes("processes", *out, *procs);
    command_handler->AddCommand(&processes);
    RunCommand run("run", *out, *procs);
    command_handler->AddCommand(&run);
    FeedCommand feed("feed", *out, *procs);
    command_handler->AddCommand(&feed);
    TerminateCommand terminate("terminate", *out, *procs);
    command_handler->AddCommand(&terminate);
    NoOperationCommand noop("no-op", *out);
    command_handler->AddCommand(&noop);

    while (!scanner->Ended() || !procs->Finished() || !out->Finished()) {
        bool did_something = scanner->Scan();
        did_something = procs->CleanFinished() || did_something;
        if (!did_something)
            Nap(100000000);
    }

    delete procs;
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
