//
//  Options.cpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 16.3.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//
// Licensed under Universal Permissive License. See License.txt.

#include "Options.hpp"
#include <ctype.h>
#include <cstring>
#include <string>
#include <vector>
#include <utility>


enum OptionValueType {
    OVT_BOOL = 1,
    OVT_INT,
    OVT_DOUBLE,
    OVT_ENUM,
    OVT_STRING
};


struct Option {
    char Letter;
    std::string LongName;
    int Index;
    bool Given;
    bool Compulsory;
    std::string Usage;
    OptionValueType Type;
    bool Flag;
    bool Bool;
    bool BoolDef;
    int Int;
    int IntDef;
    int IntMin;
    int IntMax;
    double Double;
    double DoubleDef;
    double DoubleMin;
    double DoubleMax;
    std::string String;
    std::string StringDef;
    std::vector<std::string> ValueStrings; /* Used with enum. */

    Option(char Letter, const char* LongName)
        : Letter(Letter), LongName(LongName), Index(0), Given(false),
        Compulsory(false), Type(OVT_BOOL), Bool(false), BoolDef(false),
        Int(0), IntDef(0), IntMin(0), IntMax(0),
        Double(0.0), DoubleDef(0.0), DoubleMin(0.0), DoubleMax(0.0)
    { }
    Option(char Letter, std::string LongName, int Index, bool Def)
        : Letter(Letter), LongName(LongName), Index(Index), Given(false),
        Compulsory(false), Type(OVT_BOOL), Bool(Def), BoolDef(Def),
        Int(0), IntDef(0), IntMin(0), IntMax(0),
        Double(0.0), DoubleDef(0.0), DoubleMin(0.0), DoubleMax(0.0)
    { }
    Option(char Letter, std::string LongName, int Index,
        int Def, int Min, int Max)
        : Letter(Letter), LongName(LongName), Index(Index), Given(false),
        Compulsory(false), Type(OVT_INT), Bool(false), BoolDef(false),
        Int(Def), IntDef(Def), IntMin(Min), IntMax(Max),
        Double(0.0), DoubleDef(0.0), DoubleMin(0.0), DoubleMax(0.0)
    { }
    Option(char Letter, std::string LongName, int Index,
        double Def, double Min, double Max)
        : Letter(Letter), LongName(LongName), Index(Index), Given(false),
        Compulsory(false), Type(OVT_DOUBLE), Bool(false), BoolDef(false),
        Int(0), IntDef(0), IntMin(0), IntMax(0),
        Double(Def), DoubleDef(Def), DoubleMin(Min), DoubleMax(Max)
    { }
    Option(char Letter, std::string LongName, int Index, const char* Def)
        : Letter(Letter), LongName(LongName), Index(Index), Given(false),
        Compulsory(false), Type(OVT_STRING), Bool(false), BoolDef(false),
        Int(0), IntDef(0), IntMin(0), IntMax(0),
        Double(0.0), DoubleDef(0.0), DoubleMin(0.0), DoubleMax(0.0),
        String(Def), StringDef(Def)
    { }

    std::string Names() const {
        std::string names;
        if (Letter)
            names += std::string("-") + std::string(1, Letter);
        if (Letter && !LongName.empty())
            names += "/";
        if (!LongName.empty())
            names += "--" + LongName;
        return names;
    }
};

static bool SensitiveEnums = false;
static std::string FirstDocs;
static std::vector<Option> Opts;
static std::string Docs;


void opt::CaseSensitiveEnums() {
    SensitiveEnums = true;
}

void opt::Add(char Letter, const char* LongName) {
    Opts.push_back(Option(Letter, LongName));
}

void opt::AddFlag(char Letter, const char* LongName, bool InitialValue) {
    Opts.push_back(Option(Letter, LongName));
    Opts.back().Bool = Opts.back().BoolDef = InitialValue;
}

void opt::AddValue(bool Default) {
    Opts.push_back(Option(Opts.back().Letter, Opts.back().LongName,
        Opts.back().Index + 1, Default));
};

void opt::AddValue(int Default, int Minimum, int Maximum) {
    Opts.push_back(Option(Opts.back().Letter, Opts.back().LongName,
        Opts.back().Index + 1, Default, Minimum, Maximum));
}

void opt::AddValue(double Default, double Minimum, double Maximum) {
    Opts.push_back(Option(Opts.back().Letter, Opts.back().LongName,
        Opts.back().Index + 1, Default, Minimum, Maximum));
}

void opt::AddValue(size_t Default, size_t Count, const char** Values) {
    Opts.push_back(Option(Opts.back().Letter, Opts.back().LongName,
        Opts.back().Index + 1, Values[Default]));
    Opts.back().Type = OVT_ENUM;
    for (size_t k = 0; k < Count; ++k)
        Opts.back().ValueStrings.push_back(std::string(Values[k]));
}

void opt::AddValue(const char* Default) {
    Opts.push_back(Option(Opts.back().Letter, Opts.back().LongName,
        Opts.back().Index + 1, Default));
}

void opt::Usage(const char* Instructions) {
    Opts.back().Usage +=
        std::string("\t") + std::string(Instructions) + std::string("\n");
}

void opt::Compulsory() {
    Opts.back().Compulsory = true;
}

void opt::Doc(const char* Line, bool BeforeOptions) {
    if (BeforeOptions)
        FirstDocs += std::string(Line) + std::string("\n");
    else
        Docs += std::string(Line) + std::string("\n");
}

void opt::PrintUsage(std::ostream& F, const char* ProgramName) {
    F << FirstDocs;
    if (ProgramName && strlen(ProgramName))
        F << "Usage: " << ProgramName << (Opts.empty() ? "\n" : " [options]\n");
    if (!Opts.empty())
        F << "Options:\n";
    for (size_t k = 0; k < Opts.size(); ++k) {
        const Option& opt = Opts[k];
        if (!opt.Index) {
            F << opt.Names();
            for (size_t n = k + 1; n < Opts.size() && Opts[n].Index; ++n) {
                F << ' ';
                switch (Opts[n].Type) {
                case OVT_BOOL:
                    F << "bool";
                    break;
                case OVT_INT:
                    F << "integer";
                    break;
                case OVT_DOUBLE:
                    F << "double";
                    break;
                case OVT_ENUM:
                    F << "enum";
                    break;
                case OVT_STRING:
                    F << "string";
                    break;
                }
                F << Opts[n].Index;
            }
            F << "\n" << opt.Usage;
        } else {
            F << "  ";
            switch (opt.Type) {
            case OVT_BOOL:
                F << "bool" << opt.Index << " = " <<
                    (opt.BoolDef ? "yes" : "no");
                break;
            case OVT_INT:
                F << "integer" << opt.Index << " = " << opt.IntDef;
                if (opt.IntMin != opt.IntMax)
                    F << ", [" << opt.IntMin << ", " << opt.IntMax << "]";
                break;
            case OVT_DOUBLE:
                F << "double" << opt.Index << " = " << opt.DoubleDef;
                if (opt.DoubleMin != opt.DoubleMax)
                    F << ", [" << opt.DoubleMin << ", " << opt.DoubleMax << "]";
                break;
            case OVT_ENUM:
                F << "enum" << opt.Index << " = " << opt.StringDef;
                F << " in { " << opt.ValueStrings[0];
                for (size_t k = 1; k < opt.ValueStrings.size(); ++k)
                    F << ", " << opt.ValueStrings[k];
                F << " }";
                break;
            case OVT_STRING:
                F << "string" << opt.Index;
                if (!opt.StringDef.empty())
                    F << " = " << opt.StringDef;
                break;
            }
            F << (opt.Compulsory ? "  (must be given)\n" : "\n");
            F << opt.Usage;
        }
    }
    F << "\n" << Docs << std::endl;
}


static Option* FindOption(char Letter, int Index) {
    for (Option& opt : Opts)
        if (opt.Index == Index && opt.Letter == Letter)
            return &opt;
    return nullptr;
}

static Option* FindOption(const char* LongName, int Index) {
    for (Option& opt : Opts)
        if (opt.Index == Index && !strcmp(opt.LongName.c_str(), LongName))
            return &opt;
    return nullptr;
}

#define MISSING_COMPULSORY 1
#define MISSING_VALUE 2

static int CheckValuesGivenAndCompulsory(std::ostream& Error) {
    int retval = 0;
    bool NeedsValue(false);
    std::string prev_names;
    for (const Option& opt : Opts) {
        std::string names = opt.Names();
        if (!opt.Given && opt.Compulsory) {
            retval = 1;
            Error << "Error: not given: " << names;
            if (opt.Index)
                Error << ":" << opt.Index;
            Error << "\n";
        }
        if (opt.Index) {
            NeedsValue = NeedsValue && !opt.Given;
            if (NeedsValue) {
                retval = !retval ? 6 : retval; // No value for previous option.
                Error << "Error: missing value: " << prev_names << "\n";
            }
        } else
            NeedsValue = opt.Given && !opt.Flag;
        prev_names = names;
    }
    if (NeedsValue) {
        retval = !retval ? 6 : retval; // Last option is missing values.
        Error << "Error: missing value: " << prev_names << "\n";
    }
    return 0;
}

bool opt::GivenOption(char Letter, const char* LongName, int argc, char** argv)
{
    for (int k = 1; k < argc; ++k) {
        if (argv[k][0] != '-')
            continue;
        size_t len = strlen(argv[k]);
        if (Letter && len == 2 && argv[k][1] == Letter)
            return true;
        if (LongName && len > 2 && argv[k][1] == '-'
            && !strcmp(argv[k] + 2, LongName))
            return true;
    }
    return false;
}

int opt::ParseCommandLine(int argc, char** argv, std::ostream& Error) {
    Option* Opt = nullptr;
    int Index = 0;
    int LastOptionIndex = 0;
    char* End = nullptr;
    for (int k = 1; k < argc; ++k) {
        if (!strcmp(argv[k], "--")) { // Skip to next and treat as value?
            if (++k == argc) {
                Error << "Error: -- is last value.\n";
                return 5;
            }
        } else if (argv[k][0] == '-') { // Was not skip string, maybe an option.
            size_t len = strlen(argv[k]);
            if (len < 2) {
                Error << "Error: you must give option after -\n";
                return 3;
            } else if (len == 2)
                Opt = FindOption(argv[k][1], 0);
            else
                Opt = FindOption(argv[k] + 2, 0); // Plus "--".
            if (!Opt) {
                Error << "Error: unknown option: " << argv[k] << "\n";
                return 4;
            }
            LastOptionIndex = k;
            Index = 0;
            if (Opt->Given) {
                Error << "Error: option given twice: " << Opt->Names() << "\n";
                return 2;
            }
            Opt->Given = true;
            Opt->Bool = !Opt->Bool;
            continue;
        }
        // A value.
        if (Opt == nullptr) {
            Error << "Error: value before options.\n";
            return 6;
        }
        if (Opt->Letter)
            Opt = FindOption(Opt->Letter, ++Index);
        else
            Opt = FindOption(Opt->LongName.c_str(), ++Index);
        if (!Opt) {
            Error << "Error: too many values for: " << argv[LastOptionIndex] << "\n";
            return 7;
        }
        if (!*argv[k]) // Empty string by-passes a value and uses default.
            continue;
        Opt->Given = true;
        switch (Opt->Type) {
        case OVT_BOOL:
            if (!strcmp(argv[k], "1")
                || !strcasecmp(argv[k], "yes")
                || !strcasecmp(argv[k], "true"))
                Opt->Bool = true;
            else if (!strcmp(argv[k], "0")
                || !strcasecmp(argv[k], "no")
                || !strcasecmp(argv[k], "false"))
                Opt->Bool = false;
            else {
                Error << "Error: invalid boolean value: " << argv[k] <<"\n";
                return 10;
            }
            break;
        case OVT_INT:
            End = nullptr;
            Opt->Int = (int)strtol(argv[k], &End, 0);
            if (*End) {
                Error << "Error: invalid integer value: " << argv[k] << "\n";
                return 20;
            }
            if (Opt->IntMin != Opt->IntMax &&
                (Opt->Int < Opt->IntMin || Opt->IntMax < Opt->Int))
            {
                Error << "Error: integer " << Opt->Int << " not in ["
                    << Opt->IntMin << ", " << Opt->IntMax << "]\n";
                return 21;
            }
            break;
        case OVT_DOUBLE:
            End = nullptr;
            Opt->Double = strtod(argv[k], &End);
            if (*End) {
                Error << "Error: invalid double value: " << argv[k] << "\n";
                return 30;
            }
            if (Opt->DoubleMin != Opt->DoubleMax &&
                (Opt->Double < Opt->DoubleMin || Opt->DoubleMax < Opt->Double))
            {
                Error << "Error double " << Opt->Double << " not in ["
                    << Opt->DoubleMin << ", " << Opt->DoubleMax << "]\n";
                return 31;
            }
            break;
        case OVT_ENUM:
            Opt->String = "";
            for (const std::string& val : Opt->ValueStrings)
                if ((SensitiveEnums && !strcmp(argv[k], val.c_str())) ||
                    (!SensitiveEnums && !strcasecmp(argv[k], val.c_str())))
                {
                    Opt->String = val;
                    break;
                }
            if (Opt->String == "") {
                Error << "Error: invalid enum value: " << argv[k] << " for: "
                    << argv[LastOptionIndex] << "\n";
                return 40;
            }
            break;
        case OVT_STRING:
            Opt->String = std::string(argv[k]);
            break;
        }
    }
    return CheckValuesGivenAndCompulsory(Error);
}

bool opt::Given(char Letter, int Index) {
    Option* Opt = FindOption(Letter, Index);
    return Opt->Given;
}

bool opt::Given(const char* LongName, int Index) {
    Option* Opt = FindOption(LongName, Index);
    return Opt->Given;
}

bool opt::Bool(char Letter, int Index) {
    Option* Opt = FindOption(Letter, Index);
    return Opt->Bool;
}

bool opt::Bool(const char* LongName, int Index) {
    Option* Opt = FindOption(LongName, Index);
    return Opt->Bool;
}

int opt::Int(char Letter, int Index) {
    Option* Opt = FindOption(Letter, Index);
    return Opt->Int;
}

int opt::Int(const char* LongName, int Index) {
    Option* Opt = FindOption(LongName, Index);
    return Opt->Int;
}

double opt::Double(char Letter, int Index) {
    Option* Opt = FindOption(Letter, Index);
    return Opt->Double;
}

double opt::Double(const char* LongName, int Index) {
    Option* Opt = FindOption(LongName, Index);
    return Opt->Double;
}

const std::string& opt::Enum(char Letter, int Index) {
    return opt::String(Letter, Index);
}

const std::string& opt::Enum(const char* LongName, int Index) {
    return opt::String(LongName, Index);
}

const std::string& opt::String(char Letter, int Index) {
    Option* Opt = FindOption(Letter, Index);
    return Opt->String;
}

const std::string& opt::String(const char* LongName, int Index) {
    Option* Opt = FindOption(LongName, Index);
    return Opt->String;
}

int opt::MinimumInt(char Letter, int Index) {
    Option* Opt = FindOption(Letter, Index);
    return Opt->IntMin;
}

int opt::MinimumInt(const char* LongName, int Index) {
    Option* Opt = FindOption(LongName, Index);
    return Opt->IntMin;
}

double opt::MinimumDouble(char Letter, int Index) {
    Option* Opt = FindOption(Letter, Index);
    return Opt->DoubleMin;
}

double opt::MinimumDouble(const char* LongName, int Index) {
    Option* Opt = FindOption(LongName, Index);
    return Opt->DoubleMin;
}

int opt::MaximumInt(char Letter, int Index) {
    Option* Opt = FindOption(Letter, Index);
    return Opt->IntMax;
}

int opt::MaximumInt(const char* LongName, int Index) {
    Option* Opt = FindOption(LongName, Index);
    return Opt->IntMax;
}

double opt::MaximumDouble(char Letter, int Index) {
    Option* Opt = FindOption(Letter, Index);
    return Opt->DoubleMax;
}

double opt::MaximumDouble(const char* LongName, int Index) {
    Option* Opt = FindOption(LongName, Index);
    return Opt->DoubleMax;
}

bool opt::DefaultBool(char Letter, int Index) {
    Option* Opt = FindOption(Letter, Index);
    return Opt->BoolDef;
}

bool opt::DefaultBool(const char* LongName, int Index) {
    Option* Opt = FindOption(LongName, Index);
    return Opt->BoolDef;
}

int opt::DefaultInt(char Letter, int Index) {
    Option* Opt = FindOption(Letter, Index);
    return Opt->IntDef;
}

int opt::DefaultInt(const char* LongName, int Index) {
    Option* Opt = FindOption(LongName, Index);
    return Opt->IntDef;
}

double opt::DefaultDouble(char Letter, int Index) {
    Option* Opt = FindOption(Letter, Index);
    return Opt->DoubleDef;
}

double opt::DefaultDouble(const char* LongName, int Index) {
    Option* Opt = FindOption(LongName, Index);
    return Opt->DoubleDef;
}

const std::string& opt::DefaultEnum(char Letter, int Index) {
    return opt::DefaultString(Letter, Index);
}

const std::string& opt::DefaultEnum(const char* LongName, int Index) {
    return opt::DefaultString(LongName, Index);
}

const std::string& opt::DefaultString(char Letter, int Index) {
    Option* Opt = FindOption(Letter, Index);
    return Opt->StringDef;
}

const std::string& opt::DefaultString(const char* LongName, int Index) {
    Option* Opt = FindOption(LongName, Index);
    return Opt->StringDef;
}
