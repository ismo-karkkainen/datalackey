//
//  LocalProcesses.cpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 18.10.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#include "LocalProcesses.hpp"
#include "LocalProcess.hpp"
#include "Notifications.hpp"
#include "NullValue.hpp"
#include <set>
#include <map>
#include <cstdlib>
#include <unistd.h>
#include <sys/errno.h>
#include <string.h>
#include <cassert>
#include <ctime>
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

std::vector<std::tuple<SimpleValue*,pid_t>> LocalProcesses::List() const {
    std::vector<std::tuple<SimpleValue*,pid_t>> results;
    std::lock_guard<std::mutex> lock(processes_mutex);
    for (auto proc : processes) {
        if (!proc.second->Finished())
            results.push_back(
                std::make_tuple(proc.first->Clone(), proc.second->PID()));
    }
    return results;
}

bool LocalProcesses::Terminate(const SimpleValue& Id) {
    std::lock_guard<std::mutex> lock(processes_mutex);
    SimpleValue* sv = Id.Clone();
    auto proc = processes.find(sv);
    delete sv;
    if (proc == processes.end() || proc->second->Finished())
        return false;
    proc->second->Terminate();
    return true;
}

static bool has_count(size_t Index, size_t Count, const std::string& command,
    std::vector<SimpleValue*>& Parameters, Output& Out, const SimpleValue& Id)
{
    if (Parameters.size() < Index + Count) {
        Error(Out, Id, "argument", "missing", command.c_str());
        return false;
    }
    return true;
}

static bool is_string(size_t Index, const std::string& command,
    std::vector<SimpleValue*>& Parameters, Output& Out, const SimpleValue& Id)
{
    if (!IsStringValue(Parameters[Index])) {
        Error(Out, Id, "argument", "not-string", command.c_str());
        return false;
    }
    return true;
}

static bool is_string_or_null(size_t Index, const std::string& command,
    std::vector<SimpleValue*>& Parameters, Output& Out, const SimpleValue& Id)
{
    if (IsStringValue(Parameters[Index]))
        return true;
    if (IsNullValue(Parameters[Index]))
        return true;
    Error(Out, Id, "argument", "not-string", "not-null", command.c_str());
    return false;
}

void LocalProcesses::Run(Output& Out,
    const SimpleValue& Id, std::vector<SimpleValue*>& Parameters)
{
    std::unique_lock<std::mutex> lock(processes_mutex);
    SimpleValue* sv = Id.Clone();
    auto proc = processes.find(sv);
    delete sv;
    if (proc != processes.end()) {
        Error(Out, Id, "identifier", "in-use");
        return;
    }
    lock.unlock();

    std::map<std::string,std::string> env2value; // env
    std::vector<std::string> input; // one channel in
    std::vector<std::pair<SimpleValue*,std::string>> value_name;
    std::vector<std::vector<std::string>> outputs; // multiple channel out
    std::string program_name;
    std::vector<std::string> program_args; // program

    // Temporary storage.
    bool clear_env = false;
    std::vector<std::pair<StringValue,StringValue>> label_name;
    std::string prefix, postfix;
    std::vector<std::pair<StringValue,SimpleValue*>> name_label;

    size_t k = 0;
    while (k < Parameters.size()) {
        std::string command = Parameters[k]->String();
        if (command == "env") {
            // env variable-name value
            if (!has_count(k, 3, command, Parameters, Out, Id) ||
                !is_string(k + 1, command, Parameters, Out, Id))
                return;
            std::string name = Parameters[k + 1]->String();
            if (name.find('=') != std::string::npos) {
                Error(Out, Id, "env", "argument", "invalid", name.c_str());
                return;
            }
            auto result = env2value.insert(std::pair<std::string,std::string>(
                name, Parameters[k + 2]->String()));
            if (!result.second) {
                Error(Out, Id, "env", "argument", "duplicate", name.c_str());
                return;
            }
            k += 3;
        } else if (command == "env-clear") {
            clear_env = true;
            k += 1;
        } else if (command == "input") {
            // input label name
            if (!has_count(k, 3, command, Parameters, Out, Id) ||
                !is_string(k + 1, command, Parameters, Out, Id) ||
                !is_string(k + 2, command, Parameters, Out, Id))
                return;
            // Cannot verify data presence yet since format not yet known.
            label_name.push_back(std::make_pair<StringValue,StringValue>(
                StringValue(Parameters[k + 1]->String()),
                StringValue(Parameters[k + 2]->String())));
            k += 3;
        } else if (command == "direct") {
            // direct value-(string|integer|null) name
            if (!has_count(k, 3, command, Parameters, Out, Id) ||
                !is_string(k + 2, command, Parameters, Out, Id))
                return;
            value_name.push_back(std::pair<SimpleValue*,std::string>(
                Parameters[k + 1], Parameters[k + 2]->String()));
            k += 3;
        } else if (command == "output") {
            // output name label|null
            if (!has_count(k, 3, command, Parameters, Out, Id) ||
                !is_string(k + 1, command, Parameters, Out, Id) ||
                !is_string_or_null(k + 2, command, Parameters, Out, Id))
                return;
            name_label.push_back(std::pair<StringValue,SimpleValue*>(
                StringValue(Parameters[k + 1]->String()), Parameters[k + 2]));
            k += 3;
        } else if (command == "output-prefix") {
            // output-prefix prefix-for-unknown
            if (!has_count(k, 2, command, Parameters, Out, Id) ||
                !is_string(k + 1, command, Parameters, Out, Id))
                return;
            prefix = Parameters[k + 1]->String();
            k += 2;
        } else if (command == "output-postfix") {
            // output-postfix postfix-for-unknown
            if (!has_count(k, 2, command, Parameters, Out, Id) ||
                !is_string(k + 1, command, Parameters, Out, Id))
                return;
            postfix = Parameters[k + 1]->String();
            k += 2;
        } else if (command == "channel") {
            if (!has_count(k, 4, command, Parameters, Out, Id) ||
                !is_string(k + 1, command, Parameters, Out, Id))
                return;
            std::string direction = Parameters[k + 1]->String();
            k += 2;
            if (direction == "in") {
                if (!input.empty()) {
                    Error(Out, Id, "channel", "in", "multiple");
                    return;
                }
                if (!is_string(k, command, Parameters, Out, Id) ||
                    !is_string(k + 1, command, Parameters, Out, Id))
                    return;
                std::string format = Parameters[k]->String();
                if (format != "JSON") {
                    Error(Out, Id, "argument", "unknown", command.c_str(),
                        direction.c_str());
                    return;
                }
                input.push_back(format);
                std::string channel = Parameters[k + 1]->String();
                if (channel == "stdin") {
                    input.push_back(channel);
                } else {
                    Error(Out, Id, "argument", "unknown", command.c_str(),
                        direction.c_str());
                    return;
                }
                k += 2;
            } else if (direction == "out") {
                if (!is_string(k, command, Parameters, Out, Id) ||
                    !is_string(k + 1, command, Parameters, Out, Id))
                    return;
                std::string format = Parameters[k]->String();
                if (format != "JSON" && format != "raw") {
                    Error(Out, Id, "argument", "unknown", command.c_str(),
                        direction.c_str());
                    return;
                }
                outputs.push_back(std::vector<std::string>());
                outputs.back().push_back(format);
                std::string channel = Parameters[k + 1]->String();
                if (channel == "stdout" || channel == "stderr") {
                    outputs.back().push_back(channel);
                } else {
                    Error(Out, Id, "argument", "unknown", command.c_str(),
                        direction.c_str());
                    return;
                }
                k += 2;
            } else {
                Error(Out, Id, "argument", "unknown", command.c_str());
                return;
            }
        } else if (command == "program") {
            // program executable args...
            if (!has_count(k, 2, command, Parameters, Out, Id) ||
                !is_string(k + 1, command, Parameters, Out, Id))
                return;
            program_name = Parameters[k + 1]->String();
            k += 2;
            while (k < Parameters.size())
                program_args.push_back(Parameters[k++]->String());
        } else {
            Error(Out, Id, "argument", "unknown", command.c_str());
            return;
        }
    }

    // Check that file exists and is an executable.
    errno = 0;
    if (access(program_name.c_str(), X_OK)) {
        int err = errno;
        assert(err != EINVAL);
        Error(Out, Id, "program", program_name.c_str(), strerror(err));
        return;
    }

    if (input.empty() && !(label_name.empty() && value_name.empty())) {
        Error(Out, Id, "argument", "missing", "channel", "in");
        return;
    }

    if (outputs.empty() &&
        !(name_label.empty() && prefix.empty() && postfix.empty()))
    {
        Error(Out, Id, "argument", "missing", "channel", "out");
        return;
    }

    std::set<std::string> seen_names; // Book-keeping for duplicate names.
    std::shared_ptr<std::vector<std::tuple<StringValue,std::shared_ptr<const RawData>,std::string>>> label_data_name(new std::vector<std::tuple<StringValue,std::shared_ptr<const RawData>,std::string>>());
    // Check data presence and name duplicates.
    for (auto& pair : label_name) {
        auto result = seen_names.insert(pair.second);
        if (!result.second) {
            Error(Out, Id, "input", "name", "duplicate");
            return;
        }
        StringValue s(pair.first);
        std::shared_ptr<const RawData> data(storage.ReadyData(s, input[0]));
        if (!data && !storage.Preload(s, input[0])) {
            Error(Out, Id, "input", "not-found", s.String().c_str());
            return;
        }
        label_data_name->push_back(std::make_tuple(s, data, pair.second));
    }
    // Check that direct names have no duplicates.
    for (auto& pair : value_name) {
        auto result = seen_names.insert(pair.second);
        if (!result.second) {
            Error(Out, Id, "direct", "name", "duplicate");
            return;
        }
    }

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

    StringValueMapperSimple* renamer = nullptr;
    if (!name_label.empty() || !prefix.empty() || !postfix.empty()) {
        renamer = new StringValueMapperSimple(prefix, postfix);
        for (auto& nl : name_label) {
            if (!renamer->Map(nl.first, nl.second)) {
                Error(Out, Id, "argument", "duplicate", "channel", "out");
                delete renamer;
                return;
            }
        }
    }

    Process* p = new LocalProcess(this, Out, storage, Id,
        program_name, program_args, env2value, input, label_data_name,
        value_name, outputs, renamer);
    if (!p->Run()) {
        // p has reported the error.
        delete p;
        return;
    }
    lock.lock();
    processes.insert(std::pair<SimpleValue*,Process*>(Id.Clone(), p));
}

void LocalProcesses::HasFinished(const SimpleValue& Id) {
    std::lock_guard<std::mutex> lock(to_delete_mutex);
    to_delete.push(Id.Clone());
}
