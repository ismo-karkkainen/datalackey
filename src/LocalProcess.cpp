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
#include "FileDescriptor.hpp"
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
#include <spawn.h>
#include <cassert>
#include <iostream>


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
        if (!FileDescriptor::Pipe(stdouterr_child[k][0], stdouterr_child[k][1])) {
            pm_run_error_pipe.Send(out, *id);
            return;
        }
    }
    if (uses_stdin) {
        if (!FileDescriptor::Pipe(stdin_child[0], stdin_child[1])) {
            pm_run_error_pipe.Send(out, *id);
            return;
        }
#if defined(__APPLE__)
        fcntl(stdin_child[1]->Descriptor(), F_SETNOSIGPIPE);
#endif
        int flags = fcntl(stdin_child[1]->Descriptor(), F_GETFL);
        fcntl(stdin_child[1]->Descriptor(), F_SETFL, flags | O_NONBLOCK);
        child_input = new FileDescriptorOutput(stdin_child[1]);
    } else {
        stdin_child[0].reset(new FileDescriptor());
        stdin_child[1].reset(new FileDescriptor());
        child_input = new NullOutput();
    }
    to_child = new Output(*child_input_enc, *child_input,
        notify_data, notify_process);
    // Create output handling objects for each output.
    child_output.reserve(outputs_info.size() < 2 ? 2 : outputs_info.size());
    bool used_std[2] = { false, false };
    for (auto& settings : outputs_info) {
        InputChannel* ic = nullptr;
        int fd_index = -1;
        if (settings[1] == "stdout")
            fd_index = 0;
        else if (settings[1] == "stderr")
            fd_index = 1;
        if (fd_index != -1) {
            ic = new FileDescriptorInput(stdouterr_child[fd_index][0]);
            used_std[fd_index] = true;
        }
        // Output response of raw data goes to output of launcher.
        MessageHandler* mh = MakeMessageHandler(settings[0].c_str(),
            (settings[0] == "bytes") ? out : *to_child, storage, *owner, id);
        StorageDataSink* sds = MakeStorageDataSink(
            settings[0].c_str(), storage, renamer, *to_child, id);
        InputScanner* is = MakeInputScanner(
            settings[0].c_str(), *ic, *mh, *sds, *to_child, id);
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
    // Take program_name, args, and env and turn to what posix_spawn needs.
    const char** argv_ptrs = new const char*[2 + args.size()];
    argv_ptrs[0] = program_name.c_str(); // Should be just name, no path.
    for (size_t k = 0; k < args.size(); ++k)
        argv_ptrs[k + 1] = args[k].c_str();
    argv_ptrs[1 + args.size()] = nullptr;
    const char** env_ptrs = new const char*[1 + env.size()];
    for (size_t k = 0; k < env.size(); ++k)
        env_ptrs[k] = env[k].c_str();
    env_ptrs[env.size()] = nullptr;
    // Use file actions to close pipes in child.
    posix_spawn_file_actions_t actions;
#if defined(__APPLE__)
    /*
    If program feeding commands to datalackey closes, executable is release
    build, OS is macOS 11.6 (at least that), then there is a failure in
    posix_spawn_file_actions_addclose for file descriptor 0. Hence the
    conditional close. The case is unlikely to happen in real use but this
    will enable the child to start on the retry. Otherwise the code run
    is the same as earlier.

    It might be closing 0 unless stdin_child[0] is closed is a bug and only
    manifests on macOS but I'm not spending more time on a fringe case.
    Fix limited to the platform this has manifested on.
    */
    bool close0 = true;
retry:
#endif
    posix_spawn_file_actions_init(&actions);
    if (!stdin_child[1]->Closed())
        posix_spawn_file_actions_addclose(&actions, stdin_child[1]->Descriptor());
    posix_spawn_file_actions_addclose(&actions, stdouterr_child[0][0]->Descriptor());
    posix_spawn_file_actions_addclose(&actions, stdouterr_child[1][0]->Descriptor());
#if defined(__APPLE__)
    if (close0)
#endif
    posix_spawn_file_actions_addclose(&actions, 0);
    posix_spawn_file_actions_addclose(&actions, 1);
    posix_spawn_file_actions_addclose(&actions, 2);
    if (!stdin_child[0]->Closed())
        posix_spawn_file_actions_adddup2(&actions, stdin_child[0]->Descriptor(), 0);
    posix_spawn_file_actions_adddup2(&actions, stdouterr_child[0][1]->Descriptor(), 1);
    posix_spawn_file_actions_adddup2(&actions, stdouterr_child[1][1]->Descriptor(), 2);
    if (!stdin_child[0]->Closed())
        posix_spawn_file_actions_addclose(&actions, stdin_child[0]->Descriptor());
    posix_spawn_file_actions_addclose(&actions, stdouterr_child[0][1]->Descriptor());
    posix_spawn_file_actions_addclose(&actions, stdouterr_child[1][1]->Descriptor());
    if (!directory.empty())
        posix_spawn_file_actions_addchdir_np(&actions, directory.c_str());
    int err = posix_spawn(&pid, program_name.c_str(), &actions, nullptr,
        (char *const *)argv_ptrs, (char *const *)env_ptrs);
    posix_spawn_file_actions_destroy(&actions);
#if defined(__APPLE__)
    if (err == EBADF && close0) { // Oddball case described above happened.
        close0 = false;
        goto retry;
    }
#endif
    child_start_lock.unlock();
    delete[] argv_ptrs;
    delete[] env_ptrs;
    if (err) {
        switch (err) {
        case EAGAIN:
            pm_run_error_no_processes.Send(out, *id);
            break;
        case EINVAL: // Internal error.
            assert(false);
            break;
        case E2BIG: // Too long argument list.
        case EACCES: // Can not execute, reach, etc.
        case ELOOP: // Too many symbolic links in path.
        case ENAMETOOLONG: // Name is too long.
        case ENOENT: // Executable does not exist.
        case ENOEXEC: // Not an executable.
        case ENOTDIR: // Component in path is not a directory.
        case ETXTBSY: // Shared text file open for reading or writing by process.
        case EFAULT: // Executable problem.
        case EIO: // IO error while reading from file system.
        case ENOMEM:
            pm_run_error_no_memory.Send(out, *id);
            break;
#if defined(EBADARCH)
        case EBADARCH: // No architecture for current system.
#endif
        default:
            std::cerr << "Unexpected posix_spawn err: " << err << std::endl;
            assert(false);
            break;
        }
        return;
    }
    stdin_child[0]->Close();
    stdouterr_child[0][1]->Close();
    stdouterr_child[1][1]->Close();
    //std::cerr << id->String() << ' ' << stdin_child[1]->Descriptor() << ' ' << stdouterr_child[0][0]->Descriptor() << ' ' << stdouterr_child[1][0]->Descriptor() << "\n";
    args.clear();
    env.clear();
    pm_run_running.Send(out, *id, pid); // Not necessarily notified.
    if (notify_data) {
        std::vector<std::pair<std::string, long long int>> ls = storage.List();
        for (auto& iter : ls)
            storage.NotifyStore(iter.first, iter.second, to_child);
    }
    if (notify_process) {
        std::vector<std::pair<std::unique_ptr<SimpleValue>,pid_t>> ip =
            owner->List();
        for (auto& iter : ip)
            owner->NotifyStart(*iter.first, iter.second, to_child);
    }
    owner->NotifyStart(*id, pid);
    // Keeping track of child state and inputs could be moved to LocalProcesses
    // and that can deal with it in a single thread, or multiple if several
    // processes are running simultaneously.
    ChildState child_state = Running;
    bool scanning = true;
    bool feed_open = true;
    while (child_state != None || scanning) {
        ChildState prev = child_state;
        child_state = get_child_state(child_state);
        if (prev != child_state && child_state == None)
            to_child->NoGlobalMessages(); // Child not reading anymore.
        // Scan for child output.
        bool had_data = false;
        scanning = false;
        for (auto& output : child_output) {
            if (!output)
                continue;
            if (output->scanner->Ended()) {
                output = nullptr;
                continue;
            }
            scanning = true;
            if (output->scanner->Scan())
                had_data = true;
        }
        if (feed_open && to_child->Closed()) {
            if (to_child->Failed())
                pm_run_error_input_failed.Send(out, *id);
            pm_run_input_closed.Send(out, *id);
            feed_open = false;
        }
        if (!had_data)
            Nap(20000000); // 20 ms.
    }
    owner->NotifyEnd(*id, pid);
}

void LocalProcess::runner() {
    running = true;
    real_runner();
    to_child->NoGlobalMessages();
    running = false;
    child_output.clear();
    delete child_writer;
    delete to_child;
    delete child_input;
    for (size_t k = 0; k < 2; ++k) {
        stdin_child[k]->Close();
        for (size_t n = 0; n < 2; ++n)
            stdouterr_child[k][n]->Close();
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
    : child_input_enc(nullptr), child_input(nullptr), to_child(nullptr),
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
        std::this_thread::yield();
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
    if (to_child != nullptr)
        to_child->Feed(Inputs);
}

void LocalProcess::EndFeed() {
    std::lock_guard<std::mutex> lock(child_start_mutex);
    if (to_child != nullptr)
        to_child->End();
}

bool LocalProcess::Closed() {
    std::lock_guard<std::mutex> lock(child_start_mutex);
    return to_child == nullptr || to_child->Closed();
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
