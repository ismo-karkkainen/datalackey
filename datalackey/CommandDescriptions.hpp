//
//  CommandDescriptions.hpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 4.12.2018
//  Copyright © 2018 Ismo Kärkkäinen. All rights reserved.
//

#ifndef CommandDescriptions_hpp
#define CommandDescriptions_hpp


#include "CommandDescription.hpp"
#include "Messages.hpp"
#include "Value_t.hpp"
#include <memory>
#include <vector>
#include <utility>


class ParameterlessDescription : public CommandDescription {
protected:
    bool validate(Output& Out, const SimpleValue& Id,
        std::vector<std::shared_ptr<SimpleValue>>& Arguments);

public:
    ParameterlessDescription(const char* const Name);
    void Report(OutputItem* Writer) const;
};


class StringListDescription : public CommandDescription {
private:
    bool paired;

protected:
    bool validate(Output& Out, const SimpleValue& Id,
        std::vector<std::shared_ptr<SimpleValue>>& Arguments);

public:
    StringListDescription(const char* const Name, bool Paired = false);
    void Report(OutputItem* Writer) const;
};


class ListDescription : public CommandDescription {
protected:
    bool validate(Output& Out, const SimpleValue& Id,
        std::vector<std::shared_ptr<SimpleValue>>& Arguments);

public:
    ListDescription(const char* const Name);
    void Report(OutputItem* Writer) const;
};


enum ValidateResult {
    Ok = 0,
    SomeOther,
    Missing,
    NotString,
    NotStringNull,
    OutsideSet
};

// Helper classes to feed and run command descriptions.
class SubCommand {
protected:
    const char* const name;

public:
    SubCommand(const char* const Name);
    virtual ~SubCommand();
    virtual void Report(OutputItem* Writer) const = 0;
    virtual std::pair<ValidateResult,size_t> Validate(const SimpleValue& Id,
        std::vector<std::shared_ptr<SimpleValue>>& Arguments,
        size_t Location) = 0;
    const char* const Name() const { return name; }
};


class SubCmdTuple : public SubCommand {
private:
    std::vector<std::unique_ptr<SubCommand>> parts;
    
public:
    SubCmdTuple(const char* const Name);
    void Report(OutputItem* Writer) const;
    std::pair<ValidateResult,size_t> Validate(const SimpleValue& Id,
        std::vector<std::shared_ptr<SimpleValue>>& Arguments, size_t Location);
    void Add(SubCommand* Part);
};


class SubOne : public SubCommand {
public:
    SubOne(const char* const Name);
    void Report(OutputItem* Writer) const;
};


// Only exists for common Report method.
class SubString : public SubOne {
public:
    SubString(const char* const Name);
    std::pair<ValidateResult,size_t> Validate(const SimpleValue& Id,
        std::vector<std::shared_ptr<SimpleValue>>& Arguments, size_t Location);
};


class SubStringChoice : public SubCommand {
private:
    std::vector<std::string> choices;

public:
    SubStringChoice(const char* const Name);
    void Report(OutputItem* Writer) const;
    std::pair<ValidateResult,size_t> Validate(const SimpleValue& Id,
        std::vector<std::shared_ptr<SimpleValue>>& Arguments, size_t Location);
    void Add(const char* const Choice) { choices.push_back(Choice); }
};


class SubStringNull : public SubOne {
public:
    SubStringNull(const char* const Name);
    std::pair<ValidateResult,size_t> Validate(const SimpleValue& Id,
        std::vector<std::shared_ptr<SimpleValue>>& Arguments, size_t Location);
};


class SubAny : public SubOne {
public:
    SubAny(const char* const Name);
    std::pair<ValidateResult,size_t> Validate(const SimpleValue& Id,
        std::vector<std::shared_ptr<SimpleValue>>& Arguments, size_t Location);
};



class FeedDescription : public CommandDescription {
protected:
    std::vector<std::unique_ptr<SubCommand>> subs;

    std::string angle(const char* const Str) const;
    void report_subcommands(OutputItem* Writer,
        const std::string& subcommand, const std::string& subcommands) const;
    bool validate(Output& Out, const SimpleValue& Id,
        std::vector<std::shared_ptr<SimpleValue>>& Arguments);

public:
    FeedDescription(const char* const Name);
    void Report(OutputItem* Writer) const;
};


class RunDescription : public FeedDescription {
protected:
    bool validate(Output& Out, const SimpleValue& Id,
        std::vector<std::shared_ptr<SimpleValue>>& Arguments);

public:
    RunDescription(const char* const Name);
    void Report(OutputItem* Writer) const;
};


#endif /* CommandDescriptions_hpp */
