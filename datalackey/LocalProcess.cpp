//
//  LocalProcess.cpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 21.11.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#include "LocalProcess.hpp"
#include "LocalProcesses.hpp"
#include "FileDescriptorInput.hpp"
#include "FileDescriptorOutput.hpp"
#include "Time.hpp"
#include "Notifications.hpp"
#include "JSONEncoder.hpp"
#include "StorageDataSinkJSON.hpp"
#include "InputScannerJSON.hpp"
#include "MessageRawJSON.hpp"
#include "InputScannerDiscard.hpp"
#include "InputScannerRawMessage.hpp"
#include "MessagePassThrough.hpp"
#include "Value_t.hpp"
#include "Number_t.hpp"
#include "NumberValue.hpp"
#include "NullValue.hpp"
#include <system_error>
#include <unistd.h>
#include <cerrno>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <fcntl.h>
#include <cassert>


LocalProcess::ChildOutput::ChildOutput(InputChannel* IC, MessageHandler* MH,
    StorageDataSink* SDS, InputScanner* IS)
    : channel(IC), handler(MH), sink(SDS), scanner(IS)
{
    assert(channel != nullptr);
    assert(handler != nullptr);
    assert(sink != nullptr);
    assert(scanner != nullptr);
}

LocalProcess::ChildOutput::~ChildOutput() {
    delete scanner;
    delete sink;
    delete handler;
    delete channel;
}

bool LocalProcess::kill() {
    if (pid == 0)
        return false;
    errno = 0;
    if (-1 == ::kill(pid, SIGKILL)) {
        int err = errno;
        switch (err) {
        case ESRCH: return false; // Maybe just exited normally.
        case EINVAL:
            assert(false); // Should never happen.
        case EPERM: return false; // Should not be possible as it is our child.
        default:
            return false;
        }
    }
    return true;
}

LocalProcess::ChildState LocalProcess::get_child_state(ChildState Previous) {
    if (!pid)
        return None;
    int status = 0;
    errno = 0;
    pid_t waited = waitpid(pid, &status, WNOHANG | WUNTRACED);
    int err = errno;
    if (waited == 0) { // Ok. Expected.
        return Running;
    } else if (waited == pid) {
        if (WIFEXITED(status)) {
            Message(out, *id, WEXITSTATUS(status), "exit");
            return None;
        } else if (WIFSIGNALED(status)) {
            Message(out, *id, WTERMSIG(status), "signal");
            return None;
        } else if (WIFSTOPPED(status)) {
            if (Previous != Stopped)
                Message(out, *id, WSTOPSIG(status), "stopped");
            return Stopped;
        }
    } else if (waited == -1) {
        switch (err) {
        case ECHILD: return None;
        case EINTR: return Previous; // Signal interrupt, presume no change.
        }
    }
    return Previous; // Something missed in switch?
}

void LocalProcess::real_runner() {
    // Create pipes for standard I/O, as needed.
    bool uses_stdin = false;
    if (!input_info.empty()) {
        if (input_info[0] == "JSON")
            child_input_enc = new JSONEncoder();
        if (input_info[1] == "stdin")
            uses_stdin = true;
    }
    for (int k = 0; k < 2; ++k) {
        if (-1 == pipe(stdouterr_child[k])) {
            Error(out, *id, "failure", "pipe");
            return;
        }
    }
    if (uses_stdin) {
        errno = 0;
        if (-1 == pipe(stdin_child)) {
            Error(out, *id, "failure", "pipe");
            return;
        }
        fcntl(stdin_child[1], F_SETNOSIGPIPE);
        child_input = new FileDescriptorOutput(stdin_child[1]);
    }
    // Create output handling objects for each output.
    child_output.reserve(outputs_info.size() < 2 ? 2 : outputs_info.size());
    bool used_std[2] = { false, false };
    for (auto& settings : outputs_info) {
        InputChannel* ic = nullptr;
        MessageHandler* mh = nullptr;
        StorageDataSink* sds = nullptr;
        InputScanner* is = nullptr;
        if (settings[1] == "stdout") {
            if (used_std[0]) {
                Error(out, *id, "duplicate", "stdout");
                return;
            }
            ic = new FileDescriptorInput(stdouterr_child[0][0]);
            used_std[0] = true;
        } else if (settings[1] == "stderr") {
            if (used_std[1]) {
                Error(out, *id, "duplicate", "stderr");
                return;
            }
            ic = new FileDescriptorInput(stdouterr_child[1][0]);
            used_std[1] = true;
        }
        if (settings[0] == "JSON") {
            if (!strcmp(out.Format(), "JSON")) {
                mh = new MessagePassThrough(out, *id);
                sds = new StorageDataSinkJSON(storage, out, renamer);
                is = new InputScannerJSON(*ic, *mh, *sds, out);
            } else
                assert(false);
        } else if (settings[0] == "raw") {
            if (!strcmp(out.Format(), "JSON")) {
                mh = new MessageRawJSON(out, *id);
                sds = new StorageDataSinkJSON(storage, out);
            } else
                assert(false);
            is = new InputScannerRawMessage(*ic, *mh, *sds);
        } else
            assert(false);
        child_output.push_back(std::shared_ptr<ChildOutput>(
            new ChildOutput(ic, mh, sds, is)));
    }
    // Create output discarding objects for unused child stdout and stderr.
    for (int k = 0; k < 2; ++k) {
        if (used_std[k])
            continue;
        InputChannel* ic = new FileDescriptorInput(stdouterr_child[k][0]);
        MessageHandler* mh = new MessagePassThrough(out, *id);
        StorageDataSink* sds = nullptr;
        if (!strcmp(out.Format(), "JSON")) {
            sds = new StorageDataSinkJSON(storage, out);
        } else
            assert(false);
        InputScanner* is = new InputScannerDiscard(*ic, *mh, *sds);
        child_output.push_back(std::shared_ptr<ChildOutput>(
            new ChildOutput(ic, mh, sds, is)));
    }
    // Take program_name, args, and env and turn to what execve needs.
    const char** argv_ptrs = new const char*[2 + args.size()];
    argv_ptrs[0] = program_name.c_str();
    for (size_t k = 0; k < args.size(); ++k)
        argv_ptrs[k + 1] = args[k].c_str();
    argv_ptrs[1 + args.size()] = nullptr;
    const char** env_ptrs = new const char*[1 + env.size()];
    for (size_t k = 0; k < env.size(); ++k)
        env_ptrs[k] = env[k].c_str();
    env_ptrs[env.size()] = nullptr;
    errno = 0;
    if ((pid = fork())) {
        // In parent.
        int err = errno;
        if (pid == -1) {
            if (err == EAGAIN)
                Error(out, *id, "no-processes");
            else if (err == ENOMEM)
                Error(out, *id, "no-memory");
            return;
        }
        if (stdin_child[0] != -1)
            close(stdin_child[0]);
        close(stdouterr_child[0][1]);
        close(stdouterr_child[1][1]);
        delete[] argv_ptrs;
        delete[] env_ptrs;
        args.clear();
        env.clear();
    } else {
        // In child process.
        // Connect pipe ends to standard I/O.
        int addrs[3] = { stdin_child[0], stdouterr_child[0][1], stdouterr_child[1][1] };
        for (int k = 0; k < 3; ++k) {
            if (addrs[k] != -1) {
                close(k);
                int fd = dup2(addrs[k], k);
                if (fd == -1)
                    exit(57 + k);
            }
        }
        // Start new process.
        errno = 0;
        execve(program_name.c_str(), (char *const *)argv_ptrs, (char *const *)env_ptrs);
        // The call above failed if we reach this point.
        int err = errno;
        switch (err) {
        case E2BIG:
        case EACCES:
        case ELOOP:
        case ENAMETOOLONG:
        case ENOENT:
        case ENOEXEC:
        case ENOTDIR:
        case ETXTBSY:
            exit(56); // Something parameter-related.
        case EFAULT:
        case EIO:
        case ENOMEM:
            exit(55); // Something system-related.
        }
        exit(54); // Something else.
    }
    if (child_input_enc != nullptr) { // If we are sending anything.
        child_feed = new Output(*child_input_enc, *child_input);
        child_writer = child_feed->Writable();
        *child_writer << Dictionary;
    }
    ChildState child_state = Running;
    bool scanning = !child_output.empty();
    // This is data to be sent. It is passed in pieces to prioritize reading.
    std::shared_ptr<const RawData> rd;
    size_t written = 0;
    // The value below is a good guess. On some OSes it could be queried and
    // the pipe buffer sizes set depending on how much data we appear to have.
    size_t slice_size = 32768; // Max amount written at once.
    while (child_state != None || scanning) {
        child_state = get_child_state(child_state);
        // Scan for child output. Is there anything if child quit?
        scanning = false;
        for (auto& output : child_output) {
            if (!output)
                continue;
            if (output->scanner->Ended()) {
                output = nullptr;
                continue;
            }
            scanning = true;
            output->scanner->Scan();
        }
        if (scanning)
            continue; // Child may be blocked sending data so avoid write now.
        if (child_state == Running && !label_data_name.empty()) {
            // Feed a bit of data.
            if (rd) { // We have data so send next slice.
                size_t left = rd->Size() - written;
                if (slice_size < left)
                    left = slice_size;
                child_writer->Write(
                    rd->CBegin() + written, rd->CBegin() + written + left);
                if (written + left == rd->Size()) // Reached end.
                    rd = nullptr;
            } else { // We need new data to send.
                // Get new data and either write all or first slice.
                StringValue s("");
                std::string n;
                std::tie(s, rd, n) = label_data_name.front();
                label_data_name.pop();
                if (!rd)
                    rd = storage.ReadyData(s, input_info[0].c_str());
                if (rd) { // Has data, pass on.
                    *child_writer << ValueRef<std::string>(n)
                        << RawItem;
                    written = rd->Size();
                    if (slice_size < written)
                        written = slice_size;
                    child_writer->Write(rd->CBegin(), rd->CBegin() + written);
                    if (written == rd->Size())
                        rd = nullptr;
                } else {
                    // Puts back to the queue for check later.
                    label_data_name.push(std::make_tuple(s, rd, n));
                }
            }
            if (!rd && label_data_name.empty()) {
                *child_writer << End;
                delete child_writer;
                child_writer = nullptr;
            }
            continue;
        }
        // Child is alive, scanned nothing, wrote nothing.
        Nap(20000000); // 20 ms.
    }
}

void LocalProcess::runner() {
    stdin_child[0] = stdin_child[1] = -1;
    stdouterr_child[0][0] = stdouterr_child[0][1] =
        stdouterr_child[1][0] = stdouterr_child[1][1] = -1;
    running = true;
    real_runner();
    running = false;
    child_output.clear();
    delete child_writer;
    delete child_feed;
    delete child_input;
    delete child_input_enc;
    if (stdin_child[1] != -1)
        close(stdin_child[1]);
    if (stdouterr_child[0][0] != -1)
        close(stdouterr_child[0][0]);
    if (stdouterr_child[1][0] != -1)
        close(stdouterr_child[1][0]);
    pid = 0;
    Message(out, *id, terminate ? "terminated" : "finished");
    owner->HasFinished(*id);
}

LocalProcess::LocalProcess(Processes* Owner,
    Output& StatusOut, Storage& S, const SimpleValue& Id,
    const std::string& ProgramName,
    const std::vector<std::string>& Arguments,
    const std::map<std::string,std::string>& Environment,
    const std::vector<std::string>& InputInfo,
    std::shared_ptr<std::vector<std::tuple<StringValue, std::shared_ptr<const RawData>, std::string>>> LabelDataName,
    const std::vector<std::pair<SimpleValue*,std::string>>& ValueName,
    const std::vector<std::vector<std::string>>& OutputsInfo,
    StringValueMapperSimple* Renamer)
    : child_input_enc(nullptr), child_input(nullptr), child_feed(nullptr),
    child_writer(nullptr),
    owner(Owner), out(StatusOut), storage(S), id(Id.Clone()),
    program_name(ProgramName), args(Arguments), input_info(InputInfo),
    outputs_info(OutputsInfo), renamer(Renamer),
    pid(0), worker(nullptr), running(false), terminate(false)
{
    for (auto iter : Environment)
        env.push_back(iter.first + "=" + iter.second);

    // Turn input values into data and put into queue with stored data.
    if (!input_info.empty()) {
        Encoder* enc = nullptr;
        if (input_info[0] == "JSON")
            enc = new JSONEncoder();
        else
            assert(false);
        for (auto& vn : ValueName) {
            std::shared_ptr<RawData> value(new RawData());
            if (IsStringValue(vn.first))
                enc->Encode(*value, ValueRef<std::string>(vn.first->String()));
            else {
                if (IsNumberValue(vn.first))
                    enc->Encode(
                        *value, NumberRef<long long int>(vn.first->Number()));
                else {
                    if (IsNullValue(vn.first))
                        enc->Encode(*value, Null);
                    else
                        assert(false);
                }
            }
            label_data_name.push(
                std::make_tuple(StringValue(), value, vn.second));
        }
        delete enc;
        for (auto& ldn : *LabelDataName)
            label_data_name.push(ldn);
    }
}

LocalProcess::~LocalProcess() {
    assert(!running);
    if (worker != nullptr) {
        worker->join();
        delete worker;
    }
    delete renamer;
    delete id;
}

bool LocalProcess::Run() {
    assert(worker == nullptr);
    try {
        worker = new std::thread(&LocalProcess::runner, this);
    }
    catch (const std::system_error& e) {
        Error(out, *id, "thread");
        owner->HasFinished(*id);
        return false;
    }
    catch (const std::exception& e) {
        Error(out, *id, "exception");
        owner->HasFinished(*id);
        return false;
    }
    return true;
}

bool LocalProcess::Terminate() {
    if (worker == nullptr)
        return false;
    return kill();
}

bool LocalProcess::Finished() const {
    return !running;
}

pid_t LocalProcess::PID() const {
    return pid;
}
