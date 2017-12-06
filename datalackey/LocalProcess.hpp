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
#include <vector>
#include <string>
#include <unistd.h>
#include <memory>
#include <utility>
#include <thread>
#include <queue>
#include <map>


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
    Encoder* child_input_enc;
    OutputChannel* child_input;
    int stdin_child[2];
    int stdouterr_child[2][2];
    Output* child_feed;
    OutputItem* child_writer;

    // Values obtained via constructor.
    Processes* owner;
    Output& out;
    Storage& storage;
    SimpleValue* id;
    std::string program_name;
    std::vector<std::string> args;
    std::vector<std::string> env;
    std::vector<std::string> input_info;
    std::queue<
        std::tuple<StringValue, std::shared_ptr<const RawData>, std::string>>
            label_data_name;
    std::vector<std::vector<std::string>> outputs_info;
    StringValueMapperSimple* renamer;

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
    // Probably does simple conversions.
    // Top-level pointer ownership transfers.
    LocalProcess(Processes* Owner,
        Output& StatusOut, Storage& S, const SimpleValue& Id,
        const std::string& ProgramName,
        const std::vector<std::string>& Arguments,
        const std::map<std::string,std::string>& Environment,
        const std::vector<std::string>& InputInfo,
        std::shared_ptr<std::vector<std::tuple<StringValue, std::shared_ptr<const RawData>, std::string>>> LabelDataName,
        const std::vector<std::pair<SimpleValue*,std::string>>& ValueName,
        const std::vector<std::vector<std::string>>& OutputsInfo,
        StringValueMapperSimple* Renamer);

    ~LocalProcess();

    bool Run();
    bool Terminate();
    bool Finished() const;

    pid_t PID() const;
};


#endif /* LocalProcess_hpp */
