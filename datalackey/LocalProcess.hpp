//
//  LocalProcess.hpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 31.10.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#ifndef LocalProcess_hpp
#define LocalProcess_hpp

#include "Process.hpp"
#include "Processes.hpp"
#include "InputChannel.hpp"
#include "MessageHandler.hpp"
#include "StorageDataSink.hpp"
#include "InputScanner.hpp"
#include "Encoder.hpp"
#include "OutputChannel.hpp"
#include "Output.hpp"
#include "Storage.hpp"
#include "StringValueMapperSimple.hpp"
#include "ProcessInput.hpp"
#include <vector>
#include <string>
#include <unistd.h>
#include <memory>
#include <utility>
#include <thread>
#include <queue>
#include <map>
#include <mutex>


class LocalProcess : public Process {
private:
    struct ChildOutput {
        InputChannel* channel;
        MessageHandler* handler;
        StorageDataSink* sink;
        InputScanner* scanner;

        // Ownership transfers.
        ChildOutput(InputChannel* IC, MessageHandler* MH, StorageDataSink* SDS,
            InputScanner* IS);
        ~ChildOutput();
    };
    // Created during child process start-up.
    std::vector<std::shared_ptr<ChildOutput>> child_output;
    class Encoder* child_input_enc;
    OutputChannel* child_input;
    int stdin_child[2];
    int stdouterr_child[2][2];
    Output* child_feed;
    OutputItem* child_writer;
    mutable std::mutex child_start_mutex;
    std::unique_lock<std::mutex> child_start_lock;

    // Values obtained via constructor.
    Processes* owner;
    Output& out;
    bool notify_data;
    bool notify_process;
    Storage& storage;
    SimpleValue* id;
    std::string program_name;
    std::vector<std::string> args;
    std::vector<std::string> env;
    std::string directory;
    std::vector<std::string> input_info;
    std::vector<std::vector<std::string>> outputs_info;
    StringValueMapper* renamer;

    bool kill();

    enum ChildState {
        None,
        Stopped,
        Running
    };
    ChildState get_child_state(ChildState Previous);

    pid_t pid;
    std::thread* worker;
    bool running, terminate;

    void real_runner();
    void runner();

public:
    // Renamer pointer ownership transfers.
    LocalProcess(Processes* Owner,
        Output& StatusOut, bool NotifyData, bool NotifyProcess,
        Storage& S, const SimpleValue& Id,
        const std::string& ProgramName,
        const std::vector<std::string>& Arguments,
        const std::map<std::string,std::string>& Environment,
        const std::string& Directory,
        const std::vector<std::string>& InputInfo,
        const std::vector<std::vector<std::string>>& OutputsInfo,
        StringValueMapper* Renamer);

    ~LocalProcess();

    class Encoder* EncoderClone() const;

    bool Run();
    void Feed(std::vector<std::shared_ptr<ProcessInput>>& Inputs);
    void EndFeed();
    bool Closed();
    bool Terminate();
    bool Finished() const;

    pid_t PID() const;
};


#endif /* LocalProcess_hpp */
