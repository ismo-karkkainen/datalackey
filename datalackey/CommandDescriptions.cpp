//
//  CommandDescriptions.cpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 4.12.2018
//  Copyright © 2018 Ismo Kärkkäinen. All rights reserved.
//

#include "CommandDescriptions.hpp"
#include "ReportConstants.hpp"
#include "ProcessMessages.hpp"
#include "NullValue.hpp"
#include "NumberValue.hpp"
#include "StringValue.hpp"


ParameterlessDescription::ParameterlessDescription(const char* const Name)
    : CommandDescription(Name)
{ }

void ParameterlessDescription::Report(OutputItem* Writer) const {
    *array_id_name(Writer) << Structure::End;
}

bool ParameterlessDescription::Validate(Output& Out, const SimpleValue& Id,
    std::vector<std::shared_ptr<SimpleValue>>& Arguments)
{
    if (!Arguments.empty())
        return error(CommandDescription::error_unexpected,
            Out, Id, Arguments, Arguments.size());
    return true;
}


StringListDescription::StringListDescription(
    const char* const Name, bool Paired)
    : CommandDescription(Name), paired(Paired)
{ }

void StringListDescription::Report(OutputItem* Writer) const {
    array_id_name(Writer);
    if (paired)
        *Writer << ValueRef<std::string>("string,string,...");
    else
        *Writer << ValueRef<std::string>(report::string_dots);
    *Writer << Structure::End;
}

bool StringListDescription::Validate(Output& Out, const SimpleValue& Id,
    std::vector<std::shared_ptr<SimpleValue>>& Arguments)
{
    if (Arguments.empty())
        return error(CommandDescription::error_missing, Out, Id, Arguments, 0);
    if (paired && Arguments.size() % 2 != 0)
        return error(CommandDescription::error_pairless,
            Out, Id, Arguments, Arguments.size());
    for (size_t k = 0; k < Arguments.size(); ++k) {
        StringValue* label(dynamic_cast<StringValue*>(Arguments[k].get()));
        if (label == nullptr)
            return error(CommandDescription::error_not_string,
                Out, Id, Arguments, k + 1);
    }
    return true;
}


ListDescription::ListDescription(const char* const Name)
    : CommandDescription(Name)
{ }

void ListDescription::Report(OutputItem* Writer) const {
    *array_id_name(Writer)
        << ValueRef<std::string>(report::string_integer_null_dots)
        << Structure::End;
}

bool ListDescription::Validate(Output& Out, const SimpleValue& Id,
    std::vector<std::shared_ptr<SimpleValue>>& Arguments)
{
    if (Arguments.empty())
        return error(CommandDescription::error_missing, Out, Id, Arguments, 0);
    return true;
}


SubCommand::SubCommand(const char* const Name) : name(Name) { }

SubCommand::~SubCommand() { }


SubCmdTuple::SubCmdTuple(const char* const Name) : SubCommand(Name) { }

void SubCmdTuple::Report(OutputItem* Writer) const {
    *Writer << Structure::Array
        << ValueRef<std::string>(name);
    for (const auto& part : parts)
        part->Report(Writer);
    *Writer << Structure::End;
}

std::pair<ValidateResult,size_t> SubCmdTuple::Validate(const SimpleValue& Id,
    std::vector<std::shared_ptr<SimpleValue>>& Arguments, size_t Location)
{
    if (!IsStringValue(Arguments[Location].get()))
        return std::pair<ValidateResult,size_t>(NotString, Location);
    if (strcmp(Arguments[Location]->String().c_str(), name))
        return std::pair<ValidateResult,size_t>(SomeOther, Location);
    if (Arguments.size() < Location + 1 + parts.size())
        return std::pair<ValidateResult,size_t>(Missing, Arguments.size());
    for (size_t k = 0; k < parts.size(); ++k) {
        auto result = parts[k]->Validate(Id, Arguments, Location + k + 1);
        if (result.first != Ok)
            return result;
    }
    return std::pair<ValidateResult,size_t>(Ok, Location + 1 + parts.size());
}

void SubCmdTuple::Add(SubCommand* Part) {
    parts.push_back(std::unique_ptr<SubCommand>(Part));
}


SubOne::SubOne(const char* const Name) : SubCommand(Name) { }

void SubOne::Report(OutputItem* Writer) const {
    *Writer << ValueRef<std::string>(name);
}


SubString::SubString(const char* const Name) : SubOne(Name) { }

std::pair<ValidateResult,size_t> SubString::Validate(const SimpleValue& Id,
    std::vector<std::shared_ptr<SimpleValue>>& Arguments, size_t Location)
{
    if (!IsStringValue(Arguments[Location].get()))
        return std::pair<ValidateResult,size_t>(NotString, Location);
    return std::pair<ValidateResult,size_t>(Ok, Location + 1);
}


SubStringChoice::SubStringChoice(const char* const Name) : SubCommand(Name) { }

void SubStringChoice::Report(OutputItem* Writer) const {
    std::string all;
    std::string separator = "";
    for (const auto& c : choices) {
        all += separator + c;
        separator = "|";
    }
    *Writer << ValueRef<std::string>(all);
}

std::pair<ValidateResult,size_t> SubStringChoice::Validate(
    const SimpleValue& Id,
    std::vector<std::shared_ptr<SimpleValue>>& Arguments, size_t Location)
{
    if (!IsStringValue(Arguments[Location].get()))
        return std::pair<ValidateResult,size_t>(NotString, Location);
    for (const auto& choice : choices)
        if (Arguments[Location]->String() == choice)
            return std::pair<ValidateResult,size_t>(Ok, Location + 1);
    return std::pair<ValidateResult,size_t>(OutsideSet, Location);
}


SubStringNull::SubStringNull(const char* const Name) : SubOne(Name) { }

std::pair<ValidateResult,size_t> SubStringNull::Validate(const SimpleValue& Id,
    std::vector<std::shared_ptr<SimpleValue>>& Arguments, size_t Location)
{
    if (!IsStringValue(Arguments[Location].get()) &&
        !IsNullValue(Arguments[Location].get()))
    {
        return std::pair<ValidateResult,size_t>(NotStringNull, Location);
    }
    return std::pair<ValidateResult,size_t>(Ok, Location + 1);
}


SubAny::SubAny(const char* const Name) : SubOne(Name) { }

std::pair<ValidateResult,size_t> SubAny::Validate(const SimpleValue& Id,
    std::vector<std::shared_ptr<SimpleValue>>& Arguments, size_t Location)
{
    return std::pair<ValidateResult,size_t>(Ok, Location + 1);
}


std::string FeedDescription::angle(const char* const Str) const {
    return std::string("<") + Str + ">";
}

void FeedDescription::report_subcommands(OutputItem* Writer,
    const std::string& subcommand, const std::string& subcommands) const
{
    std::string angled;
    *Writer << ValueRef<std::string>(subcommands)
        << Structure::Array
        << Structure::Array
        << ValueRef<std::string>(subcommand)
        << ValueRef<std::string>(subcommands)
        << Structure::End
        << Structure::Array
        << ValueRef<std::string>(subcommand)
        << Structure::End
        << Structure::End;
    *Writer << ValueRef<std::string>(subcommand)
        << Structure::Array;
    for (const auto& sub : subs) {
        angled = angle(sub->Name());
        *Writer << ValueRef<std::string>(angled);
    }
    *Writer << Structure::End;
    for (const auto& sub : subs) {
        angled = angle(sub->Name());
        *Writer << ValueRef<std::string>(angled);
        sub->Report(Writer);
    }
}

FeedDescription::FeedDescription(const char* const Name)
    : CommandDescription(Name)
{
    SubCmdTuple* group = new SubCmdTuple("input");
    group->Add(new SubString("source-label"));
    group->Add(new SubString("target-name"));
    subs.push_back(std::unique_ptr<SubCommand>(group));
    group = new SubCmdTuple("direct");
    group->Add(new SubAny("value-string|integer|null"));
    group->Add(new SubString("target-name"));
    subs.push_back(std::unique_ptr<SubCommand>(group));
}

void FeedDescription::Report(OutputItem* Writer) const {
    std::string angled = angle(name);
    *Writer << Structure::Dictionary
        << ValueRef<std::string>(angled)
        << Structure::Array
        << ValueRef<std::string>(report::id)
        << ValueRef<std::string>(name)
        << ValueRef<std::string>(report::process_id)
        << ValueRef<std::string>("<feed-subcommands>")
        << Structure::End;
    report_subcommands(Writer, "<feed-subcommand>", "<feed-subcommands>");
    *Writer << Structure::End;
}

bool FeedDescription::Validate(Output& Out, const SimpleValue& Id,
    std::vector<std::shared_ptr<SimpleValue>>& Arguments)
{
    if (Arguments.empty())
        return error(CommandDescription::error_missing, Out, Id, Arguments, 0);
    size_t loc = 1;
    while (loc < Arguments.size()) {
        bool found = false;
        for (const auto& sub : subs) {
            auto result = sub->Validate(Id, Arguments, loc);
            switch (result.first) {
            case Ok:
                loc = result.second;
                found = true;
                break;
            case SomeOther:
                continue;
            case Missing:
                return error(CommandDescription::error_missing,
                    Out, Id, Arguments, result.second);
            case NotString:
                return error(CommandDescription::error_not_string,
                    Out, Id, Arguments, result.second + 1);
            default:
                assert(false);
            }
            if (found)
                break;
        }
        if (!found)
            return error(CommandDescription::error_unknown,
                Out, Id, Arguments, loc + 1);
    }
    return true;
}


RunDescription::RunDescription(const char* const Name)
    : FeedDescription(Name)
{
    SubCmdTuple* group = new SubCmdTuple("env");
    group->Add(new SubString("variable-name"));
    group->Add(new SubAny("value-string|integer|null"));
    subs.push_back(std::unique_ptr<SubCommand>(group));
    group = new SubCmdTuple("env-clear");
    subs.push_back(std::unique_ptr<SubCommand>(group));
    group = new SubCmdTuple("output");
    group->Add(new SubString("result-name"));
    group->Add(new SubStringNull("label|null"));
    subs.push_back(std::unique_ptr<SubCommand>(group));
    group = new SubCmdTuple("output-prefix");
    group->Add(new SubString("prefix-for-unmapped-result-name"));
    subs.push_back(std::unique_ptr<SubCommand>(group));
    group = new SubCmdTuple("output-postfix");
    group->Add(new SubString("unmapped-result-name-postfix"));
    subs.push_back(std::unique_ptr<SubCommand>(group));
    group = new SubCmdTuple("in");
    SubStringChoice* choices = new SubStringChoice("format");
    choices->Add("JSON");
    group->Add(choices);
    choices = new SubStringChoice("channel");
    choices->Add("stdin");
    group->Add(choices);
    subs.push_back(std::unique_ptr<SubCommand>(group));
    group = new SubCmdTuple("out");
    choices = new SubStringChoice("format");
    choices->Add("JSON");
    choices->Add("bytes");
    group->Add(choices);
    choices = new SubStringChoice("channel");
    choices->Add("stdout");
    choices->Add("stderr");
    group->Add(choices);
    subs.push_back(std::unique_ptr<SubCommand>(group));
    group = new SubCmdTuple("notify");
    choices = new SubStringChoice("type");
    choices->Add("data");
    choices->Add("process");
    group->Add(choices);
    subs.push_back(std::unique_ptr<SubCommand>(group));
    group = new SubCmdTuple("end-feed");
    subs.push_back(std::unique_ptr<SubCommand>(group));
    group = new SubCmdTuple("change-directory");
    group->Add(new SubString("directory"));
    subs.push_back(std::unique_ptr<SubCommand>(group));
    group = new SubCmdTuple("program");
    group->Add(new SubString("executable"));
    subs.push_back(std::unique_ptr<SubCommand>(group));
}

void RunDescription::Report(OutputItem* Writer) const {
    std::string angled = angle(name);
    *Writer << Structure::Dictionary
        << ValueRef<std::string>(angled)
        << Structure::Array
        << ValueRef<std::string>(report::id)
        << ValueRef<std::string>(name)
        << ValueRef<std::string>("<run-subcommands>")
        << Structure::End;
    report_subcommands(Writer, "<run-subcommand>", "<run-subcommands>");
    *Writer << Structure::End;
}

bool RunDescription::Validate(Output& Out, const SimpleValue& Id,
    std::vector<std::shared_ptr<SimpleValue>>& Arguments)
{
    if (Arguments.empty())
        return error(CommandDescription::error_missing, Out, Id, Arguments, 0);
    size_t loc = 0;
    while (loc < Arguments.size()) {
        bool found = false;
        for (const auto& sub : subs) {
            auto result = sub->Validate(Id, Arguments, loc);
            switch (result.first) {
            case Ok:
                if (!strcmp(sub->Name(), "program"))
                    return true;
                loc = result.second;
                found = true;
                break;
            case SomeOther:
                break;
            case Missing:
                return error(CommandDescription::error_missing,
                    Out, Id, Arguments, result.second);
            case NotString:
                return error(CommandDescription::error_not_string,
                    Out, Id, Arguments, result.second + 1);
            case NotStringNull:
                return error(CommandDescription::error_not_string_null,
                    Out, Id, Arguments, result.second + 1);
            case OutsideSet:
                return error(CommandDescription::error_unknown,
                    Out, Id, Arguments, result.second + 1);
            default:
                assert(false);
            }
            if (found)
                break;
        }
        if (!found)
            return error(CommandDescription::error_unknown,
                Out, Id, Arguments, loc + 1);
    }
    // Program name checking happens currently later so call this "ok".
    return true; // Program was not given but otherwise valid.
}
