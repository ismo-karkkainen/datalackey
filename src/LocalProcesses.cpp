//
//  LocalProcesses.cpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 18.10.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//
// Licensed under Universal Permissive License. See License.txt.

#include "LocalProcesses.hpp"
#include "LocalProcess.hpp"
#include "ProcessMessages.hpp"
#include "NullValue.hpp"
#include "ProcessInput.hpp"
#include "JSONEncoder.hpp"
#include "File.hpp"
#include <set>
#include <map>
#include <cstdlib>
#include <unistd.h>
#include <cerrno>
#include <string.h>
#include <cassert>
#include <ctime>
#include <memory>

extern char **environ;


LocalProcesses::LocalProcesses(Storage& S)
    : storage(S)
{ }

LocalProcesses::~LocalProcesses() {
    assert(Finished());
}

bool LocalProcesses::CleanFinished() {
    std::lock_guard<std::mutex> lock(to_delete_mutex);
    if (to_delete.empty())
        return false;
    std::lock_guard<std::mutex> proc_lock(processes_mutex);
    while (!to_delete.empty()) {
        SimpleValue* id = to_delete.top();
        to_delete.pop();
        auto proc = processes.find(id);
        delete id;
        assert(proc != processes.end());
        delete proc->second;
        processes.erase(proc);
    }
    return true;
}

bool LocalProcesses::Finished() const {
    return processes.empty();
}

std::vector<std::pair<std::unique_ptr<SimpleValue>,pid_t>>
    LocalProcesses::List() const
{
    std::vector<std::pair<std::unique_ptr<SimpleValue>,pid_t>> results;
    std::lock_guard<std::mutex> lock(processes_mutex);
    for (auto proc : processes) {
        if (!proc.second->Finished())
            results.push_back(std::make_pair(
                std::unique_ptr<SimpleValue>(proc.first->Clone()),
                proc.second->PID()));
    }
    return results;
}

bool LocalProcesses::Terminate(const SimpleValue& Id) {
    SimpleValue* sv = Id.Clone();
    std::lock_guard<std::mutex> lock(processes_mutex);
    auto proc = processes.find(sv);
    delete sv;
    if (proc == processes.end())
        return false;
    if (proc->second->Finished())
        return true;
    proc->second->Terminate();
    return true;
}

std::pair<bool,std::vector<std::shared_ptr<ProcessInput>>> LocalProcesses::feed(
    Output& Out, const SimpleValue& Id,
    std::vector<std::shared_ptr<SimpleValue>>& Parameters, Encoder* E)
{
    std::unique_ptr<Encoder> enc(E);
    std::vector<std::shared_ptr<ProcessInput>> input_values;
    std::set<std::string> seen_names; // Book-keeping for duplicate names.
    size_t k = 1;
    while (k < Parameters.size()) {
        std::string command = Parameters[k]->String();
        if (command == "input") {
            // input label name
            input_values.push_back(std::shared_ptr<ProcessInput>(
                new ProcessInput(Parameters[k + 1], Parameters[k + 2])));
            k += 3;
        } else if (command == "direct") {
            // direct value-(string|integer|null) name
            input_values.push_back(std::shared_ptr<ProcessInput>(
                new ProcessInput(Parameters[k + 1], E, Parameters[k + 2])));
            k += 3;
        } else
            assert(false);
        auto result = seen_names.insert(input_values.back()->Name()->String());
        if (!result.second) {
            pm_feed_error_cmd_duplicate.Send(Out, Id, command.c_str(),
                input_values.back()->Name()->String().c_str());
            return std::pair<bool,std::vector<std::shared_ptr<ProcessInput>>>(false,std::vector<std::shared_ptr<ProcessInput>>());;
        }
    }
    if (enc && enc->Format() != nullptr)
        storage.Prepare(enc->Format(), input_values);
    std::vector<std::shared_ptr<SimpleValue>> missing;
    for (auto input : input_values)
        if (!input->Data())
            missing.push_back(input->SharedLabel());
    if (!missing.empty())
        pm_feed_error_missing.Send(Out, Id, missing);
    return std::pair<bool,std::vector<std::shared_ptr<ProcessInput>>>(
        missing.empty(), input_values);
}

bool LocalProcesses::Run(Output& Out, const SimpleValue& Id,
    std::vector<std::shared_ptr<SimpleValue>>& Parameters)
{
    std::unique_lock<std::mutex> lock(processes_mutex);
    // early check on identifier availability.
    SimpleValue* sv = Id.Clone();
    auto proc = processes.find(sv);
    delete sv;
    if (proc != processes.end()) {
        pm_run_error_identifier_in_use.Send(Out, Id);
        return false;
    }
    lock.unlock();

    std::map<std::string,std::string> env2value; // env
    std::vector<std::string> input; // one channel in
    std::vector<std::vector<std::string>> outputs; // multiple channel out
    std::string program_name;
    std::vector<std::string> program_args; // program
    bool notify_data = false;
    bool notify_process = false;
    std::string directory;

    // Temporary storage.
    bool clear_env = false;
    std::string prefix, postfix;
    std::vector<std::pair<StringValue,std::shared_ptr<SimpleValue>>> name_label;

    size_t k = 0;
    while (k < Parameters.size()) {
        std::string command = Parameters[k]->String();
        if (command == "env") {
            // env variable-name value
            std::string name = Parameters[k + 1]->String();
            if (name.find('=') != std::string::npos) {
                pm_run_error_env_invalid.Send(Out, Id, name.c_str());
                return false;
            }
            auto result = env2value.insert(std::pair<std::string,std::string>(
                name, Parameters[k + 2]->String()));
            if (!result.second) {
                pm_run_error_env_duplicate.Send(Out, Id, name.c_str());
                return false;
            }
            k += 3;
        } else if (command == "env-clear") {
            clear_env = true;
            k += 1;
        } else if (command == "output") {
            // output name label|null
            name_label.push_back(
                std::pair<StringValue,std::shared_ptr<SimpleValue>>(
                    StringValue(Parameters[k + 1]->String()),
                    Parameters[k + 2]));
            k += 3;
        } else if (command == "output-prefix") {
            // output-prefix prefix-for-unknown
            prefix = Parameters[k + 1]->String();
            k += 2;
        } else if (command == "output-postfix") {
            // output-postfix postfix-for-unknown
            postfix = Parameters[k + 1]->String();
            k += 2;
        } else if (command == "in") {
            ++k;
            if (!input.empty()) {
                pm_run_error_in_multiple.Send(Out, Id);
                return false;
            }
            std::string format = Parameters[k]->String();
            input.push_back(format);
            std::string channel = Parameters[k + 1]->String();
            input.push_back(channel);
            k += 2;
        } else if (command == "out") {
            ++k;
            std::string format = Parameters[k]->String();
            std::string channel = Parameters[k + 1]->String();
            for (const auto& earlier : outputs)
                if (earlier[1] == channel) {
                    pm_run_error_out_duplicate.Send(Out, Id, channel.c_str());
                    return false;
                }
            outputs.push_back(std::vector<std::string>());
            outputs.back().push_back(format);
            outputs.back().push_back(channel);
            k += 2;
        } else if (command == "notify") {
            if (Parameters[k + 1]->String() == "data")
                notify_data = true;
            else if (Parameters[k + 1]->String() == "process")
                notify_process = true;
            else
                assert(false);
            k += 2;
        } else if (command == "change-directory") {
            directory = Parameters[k + 1]->String();
            k += 2;
        } else if (command == "program") {
            // program executable args...
            program_name = Parameters[k + 1]->String();
            k += 2;
            while (k < Parameters.size())
                program_args.push_back(Parameters[k++]->String());
        } else
            assert(false);
    }

    if ((notify_data || notify_process) && input.empty()) {
        pm_run_error_notify_no_input.Send(Out, Id);
        return false;
    }

    if (!directory.empty()) {
        std::string absolute = AbsoluteDirectory(directory);
        if (absolute.empty()) {
            pm_run_error_cd_errmsg.Send(
                Out, Id, directory.c_str(), strerror(errno));
            return false;
        }
        directory = absolute;
    }

    std::unique_ptr<StringValueMapperSimple> renamer;
    if (!name_label.empty() || !prefix.empty() || !postfix.empty()) {
        renamer.reset(new StringValueMapperSimple(prefix, postfix));
        for (auto& nl : name_label) {
            if (!renamer->Map(nl.first, nl.second)) {
                pm_run_error_output_duplicate.Send(
                    Out, Id, nl.first.String().c_str());
                return false;
            }
        }
    }

    if (outputs.empty() &&
        !(name_label.empty() && prefix.empty() && postfix.empty()))
    {
        pm_run_error_out_missing.Send(Out, Id);
        return false;
    }

    std::string absolute = AbsoluteFile(program_name, true);
    if (absolute.empty()) {
        if (errno)
            pm_run_error_program_errmsg.Send(
                Out, Id, program_name.c_str(), strerror(errno));
        else
            pm_run_error_program.Send(Out, Id, program_name.c_str());
        return false;
    }
    program_name = absolute;

    if (!clear_env) {
        // Copy environment to mapping but do not over-write values.
        for (char** ev = environ; *ev != nullptr; ++ev) {
            std::string env(*ev);
            std::string val;
            size_t loc = env.find('=');
            if (loc != std::string::npos) {
                val = std::string(env, loc + 1, env.size() - (loc + 1));
                env.erase(loc);
            }
            auto iter = env2value.find(env);
            if (iter == env2value.end()) {
                env2value.insert(std::pair<std::string,std::string>(env, val));
            }
        }
    }

    // Other process with same identifier may just have started. Check again.
    lock.lock();
    sv = Id.Clone();
    proc = processes.find(sv);
    delete sv;
    if (proc != processes.end()) {
        pm_run_error_identifier_in_use.Send(Out, Id);
        return false;
    }
    Process* p = new LocalProcess(this, Out, notify_data, notify_process,
        storage, Id, program_name, program_args, env2value, directory,
        input, outputs, renamer.release());

    if (p->Run())
        processes.insert(std::pair<SimpleValue*,Process*>(Id.Clone(), p));
    else // Error has been reported.
        delete p; // The done message is sent by p destructor.
    return true;
}

void LocalProcesses::Feed(Output& Out, const SimpleValue& Id,
    std::vector<std::shared_ptr<SimpleValue>>& Parameters)
{
    std::unique_lock<std::mutex> lock(processes_mutex);
    auto proc = processes.find(Parameters[0].get());
    if (proc == processes.end()) {
        pm_feed_error_not_found.Send(Out, Id);
        return;
    }
    lock.unlock();
    if (proc->second->Closed()) {
        pm_feed_error_closed.Send(Out, Id);
        return;
    }
    auto inputs = feed(Out, Id, Parameters, proc->second->EncoderClone());
    if (inputs.first)
        proc->second->Feed(inputs.second);
}

void LocalProcesses::EndFeed(Output& Out, const SimpleValue& Id,
    std::vector<std::shared_ptr<SimpleValue>>& Parameters)
{
    std::vector<std::shared_ptr<SimpleValue>> not_found, not_open;
    std::unique_lock<std::mutex> lock(processes_mutex);
    for (auto& param : Parameters) {
        auto proc = processes.find(param.get());
        if (proc != processes.end()) {
            if (!proc->second->Closed())
                proc->second->EndFeed();
            else
                not_open.push_back(param);
        } else
            not_found.push_back(param);
    }
    lock.unlock();
    if (!not_found.empty())
        pm_end_feed_missing.Send(Out, Id, not_found);
    if (!not_open.empty())
        pm_end_feed_not_open.Send(Out, Id, not_open);
}

void LocalProcesses::HasFinished(const SimpleValue& Id) {
    std::lock_guard<std::mutex> lock(to_delete_mutex);
    to_delete.push(Id.Clone());
}
