//
//  LocalProcess.cpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 21.11.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//
// Licensed under Universal Permissive License. See License.txt.

#include "LocalProcess.hpp"
#include "LocalProcesses.hpp"
#include "FileDescriptorInput.hpp"
#include "FileDescriptorOutput.hpp"
#include "Time.hpp"
#include "Messages.hpp"
#include "ProcessMessages.hpp"
#include "JSONEncoder.hpp"
#include "NullEncoder.hpp"
#include "NullOutput.hpp"
#include "Factories.hpp"
#include "Value_t.hpp"
#include "Number_t.hpp"
#include "NumberValue.hpp"
#include "NullValue.hpp"
#include "OutputCollection.hpp"
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
        if (Previous == Stopped)
            pm_run_continued.Send(out, *id);
        return Running;
    } else if (waited == pid) {
        if (WIFEXITED(status)) {
            pm_run_exit.Send(out, *id, WEXITSTATUS(status));
            return None;
        } else if (WIFSIGNALED(status)) {
            pm_run_signal.Send(out, *id, WTERMSIG(status));
            return None;
        } else if (WIFSTOPPED(status)) {
            if (Previous != Stopped)
                pm_run_stopped.Send(out, *id, WSTOPSIG(status));
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
        if (input_info[1] == "stdin")
            uses_stdin = true;
    }
    for (int k = 0; k < 2; ++k) {
        if (-1 == pipe(stdouterr_child[k])) {
            pm_run_error_pipe.Send(out, *id);
            return;
        }
    }
    if (uses_stdin) {
        errno = 0;
        if (-1 == pipe(stdin_child)) {
            pm_run_error_pipe.Send(out, *id);
            return;
        }
#if defined(__APPLE__)
        fcntl(stdin_child[1], F_SETNOSIGPIPE);
#endif
        int flags = fcntl(stdin_child[1], F_GETFL);
        fcntl(stdin_child[1], F_SETFL, flags | O_NONBLOCK);
        child_input = new FileDescriptorOutput(stdin_child[1]);
    } else {
        child_input = new NullOutput();
    }
    child_feed = new Output(*child_input_enc, *child_input,
        notify_data, notify_process);
    // Create output handling objects for each output.
    child_output.reserve(outputs_info.size() < 2 ? 2 : outputs_info.size());
    bool used_std[2] = { false, false };
    for (auto& settings : outputs_info) {
        InputChannel* ic = nullptr;
        if (settings[1] == "stdout") {
            ic = new FileDescriptorInput(stdouterr_child[0][0]);
            used_std[0] = true;
        } else if (settings[1] == "stderr") {
            ic = new FileDescriptorInput(stdouterr_child[1][0]);
            used_std[1] = true;
        }
        // Output response of raw data goes to output of launcher.
        MessageHandler* mh = MakeMessageHandler(settings[0].c_str(),
            (settings[0] == "bytes") ? out : *child_feed, storage, *owner, id);
        StorageDataSink* sds = MakeStorageDataSink(
            settings[0].c_str(), storage, renamer, *child_feed, id);
        InputScanner* is = MakeInputScanner(
            settings[0].c_str(), *ic, *mh, *sds, *child_feed, id);
        child_output.push_back(std::shared_ptr<ChildOutput>(
            new ChildOutput(ic, mh, sds, is)));
    }
    // Create output discarding objects for unused child stdout and stderr.
    for (int k = 0; k < 2; ++k) {
        if (used_std[k])
            continue;
        InputChannel* ic = new FileDescriptorInput(stdouterr_child[k][0]);
        MessageHandler* mh =
            MakeMessageHandler(out.Format(), out, storage, *owner, id);
        StorageDataSink* sds =
            MakeStorageDataSink(out.Format(), storage, renamer, out, id);
        InputScanner* is = MakeInputScanner(nullptr, *ic, *mh, *sds, out, id);
        child_output.push_back(std::shared_ptr<ChildOutput>(
            new ChildOutput(ic, mh, sds, is)));
    }
    child_start_lock.unlock();
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
                pm_run_error_no_processes.Send(out, *id);
            else if (err == ENOMEM)
                pm_run_error_no_memory.Send(out, *id);
            return;
        }
        if (stdin_child[0] != -1)
            close(stdin_child[0]);
        close(stdouterr_child[0][1]);
        close(stdouterr_child[1][1]);
        stdin_child[0] = stdouterr_child[0][1] = stdouterr_child[1][1] = -1;
        delete[] argv_ptrs;
        delete[] env_ptrs;
        args.clear();
        env.clear();
    } else {
        // In child process.
        // The ends we do not need.
        if (stdin_child[1] != -1)
            close(stdin_child[1]);
        close(stdouterr_child[0][0]);
        close(stdouterr_child[1][0]);
        // Connect pipe ends to standard I/O and close original pipe ends.
        int addrs[3] = {
            stdin_child[0], stdouterr_child[0][1], stdouterr_child[1][1] };
        for (int k = 0; k < 3; ++k) {
            if (addrs[k] != -1) {
                close(k);
                int fd = dup2(addrs[k], k);
                if (fd == -1)
                    exit(57 + k);
                close(addrs[k]);
            }
        }
        // Change directory.
        if (!directory.empty() && -1 == chdir(directory.c_str()))
            exit(53);
        // Start the desired process.
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
    pm_run_running.Send(out, *id, pid); // Not necessarily notified.
    if (notify_data) {
        std::vector<std::pair<std::string, long long int>> ls = storage.List();
        for (auto& iter : ls)
            storage.NotifyStore(iter.first, iter.second, child_feed);
    }
    if (notify_process) {
        std::vector<std::pair<std::unique_ptr<SimpleValue>,pid_t>> ip =
            owner->List();
        for (auto& iter : ip)
            owner->NotifyStart(*iter.first, iter.second, child_feed);
    }
    owner->NotifyStart(*id, pid);
    ChildState child_state = Running;
    bool scanning = !child_output.empty();
    bool feed_open = !child_feed->Failed();
    while (child_state != None || scanning) {
        ChildState prev = child_state;
        child_state = get_child_state(child_state);
        if (prev != child_state && child_state == None)
            child_feed->NoGlobalMessages(); // Child not reading anymore.
        // Scan for child output.
        scanning = false;
        for (auto& output : child_output) {
            if (!output)
                continue;
            if (output->scanner->Ended()) {
                output = nullptr;
                continue;
            }
            if (output->scanner->Scan() > 0)
                scanning = true;
        }
        if (feed_open && child_feed->Closed()) {
            if (child_feed->Failed())
                pm_run_error_input_failed.Send(out, *id);
            pm_run_input_closed.Send(out, *id);
            feed_open = false;
        }
        if (!scanning)
            Nap(20000000); // 20 ms.
    }
    owner->NotifyEnd(*id, pid);
}

void LocalProcess::runner() {
    stdin_child[0] = stdin_child[1] = -1;
    stdouterr_child[0][0] = stdouterr_child[0][1] =
        stdouterr_child[1][0] = stdouterr_child[1][1] = -1;
    running = true;
    real_runner();
    child_feed->NoGlobalMessages();
    running = false;
    child_output.clear();
    delete child_writer;
    delete child_feed;
    delete child_input;
    for (size_t k = 0; k < 2; ++k) {
        if (stdin_child[k] != -1)
            close(stdin_child[k]);
        for (size_t n = 0; n < 2; ++n)
            if (stdouterr_child[k][n] != -1)
                close(stdouterr_child[k][n]);
    }
    if (terminate)
        pm_run_terminated.Send(out, *id);
    else
        pm_run_finished.Send(out, *id);
    pid = 0;
    owner->HasFinished(*id);
}

LocalProcess::LocalProcess(Processes* Owner,
    Output& StatusOut, bool NotifyData, bool NotifyProcess,
    Storage& S, const SimpleValue& Id,
    const std::string& ProgramName,
    const std::vector<std::string>& Arguments,
    const std::map<std::string,std::string>& Environment,
    const std::string& Directory,
    const std::vector<std::string>& InputInfo,
    const std::vector<std::vector<std::string>>& OutputsInfo,
    StringValueMapper* Renamer)
    : child_input_enc(nullptr), child_input(nullptr), child_feed(nullptr),
    child_writer(nullptr), child_start_lock(child_start_mutex, std::defer_lock),
    owner(Owner), out(StatusOut), notify_data(NotifyData),
    notify_process(NotifyProcess), storage(S),
    id(Id.Clone()), program_name(ProgramName), args(Arguments),
    directory(Directory), input_info(InputInfo),
    outputs_info(OutputsInfo), renamer(Renamer),
    pid(0), worker(nullptr), running(false), terminate(false)
{
    for (auto iter : Environment)
        env.push_back(iter.first + "=" + iter.second);
    if (!input_info.empty()) {
        if (input_info[0] == "JSON")
            child_input_enc = new JSONEncoder();
    } else {
        child_input_enc = new NullEncoder();
        notify_data = false;
        notify_process = false;
    }
}

LocalProcess::~LocalProcess() {
    assert(!running);
    msg_done.Send(out, *id);
    if (worker != nullptr) {
        worker->join();
        delete worker;
    }
    delete renamer;
    delete id;
    delete child_input_enc;
}

Encoder* LocalProcess::EncoderClone() const {
    return child_input_enc->Clone();
}

bool LocalProcess::Run() {
    assert(worker == nullptr);
    try {
        child_start_lock.lock();
        worker = new std::thread(&LocalProcess::runner, this);
    }
    catch (const std::system_error& e) {
        child_start_lock.unlock();
        pm_run_error_no_thread.Send(out, *id);
        owner->HasFinished(*id);
        return false;
    }
    catch (const std::exception& e) {
        child_start_lock.unlock();
        pm_run_error_exception.Send(out, *id);
        owner->HasFinished(*id);
        return false;
    }
    return true;
}

void LocalProcess::Feed(std::vector<std::shared_ptr<ProcessInput>>& Inputs) {
    std::lock_guard<std::mutex> lock(child_start_mutex);
    if (child_feed != nullptr)
        child_feed->Feed(Inputs);
}

void LocalProcess::EndFeed() {
    std::lock_guard<std::mutex> lock(child_start_mutex);
    if (child_feed != nullptr)
        child_feed->End();
}

bool LocalProcess::Closed() {
    std::lock_guard<std::mutex> lock(child_start_mutex);
    return child_feed == nullptr || child_feed->Closed();
}

bool LocalProcess::Terminate() {
    std::lock_guard<std::mutex> lock(child_start_mutex);
    if (worker == nullptr)
        return false;
    return kill();
}

bool LocalProcess::Finished() const {
    return !running;
}

pid_t LocalProcess::PID() const {
    std::lock_guard<std::mutex> lock(child_start_mutex);
    return pid;
}