//
//  Options.hpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 16.3.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#if !defined(OPTIONS_HPP)
#define OPTIONS_HPP

#include <iostream>


/* These functions are used to pass information about the allowed contents of
 * the command line. Anything passed as argument can be deallocated or reused
 * after the call.
 *
 * First you have to define what command-line can hold, which is done with
 * Add. Any values following them will be given in the order of
 * occurrence with AddValue. Usage attachs to the last option or
 * value a document string and Compulsory flags the option or value
 * as compulsory.
 *
 * Values and information whether the option was given are obtained with the
 * Value functions where you can query the value with the letter or the
 * long name of the option along with the index (option is 0 and first value
 * is 1).
 */

namespace opt {

/* Sets enum value comparisons to be case sensitive, insensitive by default.
   Only affects the parsing of the command line. */
void CaseSensitiveEnums();

// Adds option, only for options with values.
void Add(char Letter, const char* LongName);
// Adds option that is intended as a boolean switch. May have values, too.
void AddFlag(char Letter, const char* LongName, bool InitialValue);

// These add values after the last added option and limits, if applicable.
void AddValue(bool Default);
void AddValue(int Default, int Minimum = 0, int Maximum = 0);
void AddValue(double Default, double Minimum = 0.0, double Maximum = 0.0);
void AddValue(const char* Default);
// Values is an array of strings that are the allowed values.
void AddValue(size_t Default, size_t Count, const char** Values);

// Intended to add a line to the description of the option. Each line will
// be prepended by tab and appended by newline so stay below 70 chars max.
void Usage(const char* Instructions);
// Marks the last given option or its value so that it must be
// given by the user.
void Compulsory();


// Adds some general documentation, works like Usage but without indentation.
void Doc(const char* Line, bool BeforeOptions = false);

// Checks if the option has been given before parsing. Use to check if user
// has raquested help before parsing to avoid error messages.
bool GivenOption(char Letter, const char* LongName, int argc, char** argv);
// Parses the command line, call after using the previous ones.
int ParseCommandLine(int argc, char** argv, std::ostream& Error);

void PrintUsage(std::ostream& F, const char* ProgramName = 0);

/* These return information about values, what was given by the user, etc.
 * There are two versions for each, depending on if you want to access the
 * value using letter or long name. Index 0 is the option, 1 is first value.
 */

bool Given(char Letter, int Index = 0);
bool Given(const char* LongName, int Index = 0);

bool Bool(char Letter, int Index);
bool Bool(const char* LongName, int Index);
int Int(char Letter, int Index);
int Int(const char* LongName, int Index);
double Double(char Letter, int Index);
double Double(const char* LongName, int Index);
const std::string& Enum(char Letter, int Index);
const std::string& Enum(const char* LongName, int Index);
const std::string& String(char Letter, int Index);
const std::string& String(const char* LongName, int Index);

// These are used to query minima, maxima and default values.

int MinimumInt(char Letter, int Index);
int MinimumInt(const char* LongName, int Index);
double MinimumDouble(char Letter, int Index);
double MinimumDouble(const char* LongName, int Index);

int MaximumInt(char Letter, int Index);
int MaximumInt(const char* LongName, int Index);
double MaximumDouble(char Letter, int Index);
double MaximumDouble(const char* LongName, int Index);

bool DefaultBool(char Letter, int Index);
bool DefaultBool(const char* LongName, int Index);
int DefaultInt(char Letter, int Index);
int DefaultInt(const char* LongName, int Index);
double DefaultDouble(char Letter, int Index);
double DefaultDouble(const char* LongName, int Index);
const std::string& DefaultEnum(char Letter, int Index);
const std::string& DefaultEnum(const char* LongName, int Index);
const std::string& DefaultString(char Letter, int Index);
const std::string& DefaultString(const char* LongName, int Index);


}; // namespace

#endif
