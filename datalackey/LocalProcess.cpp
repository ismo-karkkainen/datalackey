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
#include "CommandHandlerJSON.hpp"
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

bool LocalProcess::real_runner() {
    // Create pipes for standard I/O, as needed.
    bool uses_stdin = false;
    if (!input_info.empty()) {
        if (input_info[1] == "stdin")
            uses_stdin = true;
    }
    for (int k = 0; k < 2; ++k) {
        if (-1 == pipe(stdouterr_child[k])) {
            Error(out, *id, "failure", "pipe");
            return false;
        }
    }
    if (uses_stdin) {
        errno = 0;
        if (-1 == pipe(stdin_child)) {
            Error(out, *id, "failure", "pipe");
            return false;
        }
        fcntl(stdin_child[1], F_SETNOSIGPIPE);
        int flags = fcntl(stdin_child[1], F_GETFL);
        fcntl(stdin_child[1], F_SETFL, flags | O_NONBLOCK);
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
                return false;
            }
            ic = new FileDescriptorInput(stdouterr_child[0][0]);
            used_std[0] = true;
        } else if (settings[1] == "stderr") {
            if (used_std[1]) {
                Error(out, *id, "duplicate", "stderr");
                return false;
            }
            ic = new FileDescriptorInput(stdouterr_child[1][0]);
            used_std[1] = true;
        }
        if (settings[0] == "JSON") {
            if (!strcmp(out.Format(), "JSON")) {
                mh = new CommandHandlerJSON(out);
                sds = new StorageDataSinkJSON(storage, id, out, renamer);
                is = new InputScannerJSON(*ic, *mh, *sds, out, id);
            } else
                assert(false);
        } else if (settings[0] == "raw") {
            if (!strcmp(out.Format(), "JSON")) {
                mh = new MessageRawJSON(out, *id);
                sds = new StorageDataSinkJSON(storage, id, out);
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
        // Input is discarded so only matching format matters.
        MessageHandler* mh = nullptr;
        StorageDataSink* sds = nullptr;
        if (!strcmp(out.Format(), "JSON")) {
            mh = new MessageRawJSON(out, *id);
            sds = new StorageDataSinkJSON(storage, id, out);
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
            return false;
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
        int addrs[3] = {
            stdin_child[0], stdouterr_child[0][1], stdouterr_child[1][1] };
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
        execve(program_name.c_str(), (char *const *)argv_ptrs,
            (char *const *)env_ptrs);
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
    if (child_input_enc != nullptr)
        child_feed = new Output(*child_input_enc, *child_input, notify_data);
    ChildState child_state = Running;
    bool scanning = !child_output.empty();
    while (child_state != None || scanning) {
        ChildState prev = child_state;
        child_state = get_child_state(child_state);
        if (prev != child_state && child_state == None && child_feed != nullptr)
            child_feed->NoGlobalMessages(); // Not reading anymore.
        // Scan for child output.
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
        if (!scanning)
            Nap(20000000); // 20 ms.
    }
    return false;
}

void LocalProcess::runner() {
    stdin_child[0] = stdin_child[1] = -1;
    stdouterr_child[0][0] = stdouterr_child[0][1] =
        stdouterr_child[1][0] = stdouterr_child[1][1] = -1;
    running = true;
    if (real_runner()) {
        kill(); // We failed to send all input.
    }
    if (child_feed)
        child_feed->NoGlobalMessages();
    running = false;
    child_output.clear();
    delete child_writer;
    delete child_feed;
    delete child_input;
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
    Output& StatusOut, bool NotifyData, Storage& S, const SimpleValue& Id,
    const std::string& ProgramName,
    const std::vector<std::string>& Arguments,
    const std::map<std::string,std::string>& Environment,
    const std::vector<std::string>& InputInfo,
    const std::vector<std::vector<std::string>>& OutputsInfo,
    StringValueMapper* Renamer)
    : child_input_enc(nullptr), child_input(nullptr), child_feed(nullptr),
    child_writer(nullptr),
    owner(Owner), out(StatusOut), notify_data(NotifyData), storage(S),
    id(Id.Clone()), program_name(ProgramName), args(Arguments),
    input_info(InputInfo),
    outputs_info(OutputsInfo), renamer(Renamer),
    pid(0), worker(nullptr), running(false), terminate(false)
{
    for (auto iter : Environment)
        env.push_back(iter.first + "=" + iter.second);
    if (!input_info.empty()) {
        if (input_info[0] == "JSON")
            child_input_enc = new JSONEncoder();
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
    delete child_input_enc;
}

Encoder* LocalProcess::Encoder() const {
    if (child_input_enc != nullptr)
        return child_input_enc->Clone();
    return nullptr;
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

void LocalProcess::Feed(std::vector<std::shared_ptr<ProcessInput>>& Inputs) {
    std::lock_guard<std::mutex> lock(input_sets_mutex);
    // Input values in one set are surrounded by nullptrs.
    inputs.push(std::shared_ptr<ProcessInput>());
    for (auto input : Inputs)
        inputs.push(input);
    inputs.push(std::shared_ptr<ProcessInput>());
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
