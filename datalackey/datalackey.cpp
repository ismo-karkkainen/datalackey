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
#include "Factories.hpp"
#include "LocalProcesses.hpp"
#include "MemoryStorage.hpp"
#include "DirectoryStorage.hpp"
#include "Options.hpp"
#include "Time.hpp"
#include "File.hpp"
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <cstring>
#include <ctime>
#include <limits>
#include <iostream>
#include <cassert>

static const char* inputs[1] = { "stdin" };
static const char* outputs[2] = { "stdout", "stderr" };
static const char* formats[1] = { "JSON" };
static const char* permissions[] = { "600", "660", "666" };

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
    opt::Add('d', "directory");
    opt::Usage("Store data in directory.");
    opt::AddValue("");
    opt::Usage("Directory under which to store data directory.");
    opt::Add('p', "permissions");
    opt::Usage("Permission combination for stored files.");
    opt::AddValue(0, 3, permissions);
    opt::Usage("Indicates read and write to user, group, other.");
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
    if (opt::Given("directory", 1)) {
        mode_t mode = S_IRUSR | S_IWUSR;
        mode_t dirmode = S_IXUSR;
        if (opt::Enum("permissions", 1) == permissions[1]) {
            mode |= S_IRGRP | S_IWGRP;
            dirmode |= S_IXGRP;
        } else if (opt::Enum("permissions", 1) == permissions[2]) {
            mode |= S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;
            dirmode |= S_IXGRP | S_IXOTH;
        }
        dirmode |= mode;
        std::string dir = opt::String("directory", 1);
        while (!dir.empty() && dir.back() == '/')
            dir.pop_back();
        dir = AbsoluteDirectory(dir);
        if (dir.empty())
            return 10;
        // Check existence and create ".datalackey" as needed.
        dir.push_back('/');
        dir += ".datalackey";
        if (AbsoluteDirectory(dir).empty()) {
            if (-1 == mkdir(dir.c_str(), dirmode))
                return 11;
        }
        dir.push_back('/');
        DirectoryStorage* ds = new DirectoryStorage(dir, mode);
        DataGroup::SetDataOwnerGenerator(*ds);
        storage = ds;
        if (!ds->IsValid())
            return 12;
    } else if (opt::Bool("memory", 0)) {
        MemoryStorage* ms = new MemoryStorage();
        DataGroup::SetDataOwnerGenerator(*ms);
        storage = ms;
    } else {
        return 1;
    }
    assert(storage != nullptr);

    LocalProcesses* procs = new LocalProcesses(*storage);

    OutputChannel* out_channel = nullptr;
    std::string choice = opt::String("command-out", 1);
    if (choice == "stdout")
        out_channel = new FileDescriptorOutput(1);
    else if (choice == "stderr")
        out_channel = new FileDescriptorOutput(2);
    else
        return 2;
    assert(out_channel != nullptr);

    Encoder* enc = nullptr;
    choice = opt::String("command-out", 2);
    if (choice == "JSON")
        enc = new JSONEncoder();
    else
        return 3;
    assert(enc != nullptr);

    Output* out = new Output(*enc, *out_channel);
    choice = opt::String("command-in", 1);
    InputChannel* in_channel = nullptr;
    if (choice == "stdin")
        in_channel = new FileDescriptorInput();
    else
        return 4;
    assert(in_channel != nullptr);

    choice = opt::String("command-in", 2);
    StorageDataSink* sink =
        MakeStorageDataSink(choice.c_str(), *storage, nullptr, *out, nullptr);
    assert(sink != nullptr);
    MessageHandler* command_handler =
        MakeMessageHandler(choice.c_str(), *out, *storage, *procs, nullptr);
    assert(command_handler != nullptr);
    InputScanner* scanner = MakeInputScanner(
        choice.c_str(), *in_channel, *command_handler, *sink, *out, nullptr);
    assert(scanner != nullptr);

    while (!scanner->Ended() || !procs->Finished() || !out->Finished()) {
        bool did_something = scanner->Scan();
        did_something = procs->CleanFinished() || did_something;
        if (!did_something)
            Nap(20000000); // 20 ms.
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
