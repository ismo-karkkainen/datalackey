//
//  CommandDescription.hpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 4.12.2018
//  Copyright © 2018 Ismo Kärkkäinen. All rights reserved.
//
// Licensed under Universal Permissive License. See License.txt.

#ifndef CommandDescription_hpp
#define CommandDescription_hpp


#include "OutputItem.hpp"
#include "Messages.hpp"
#include <cstddef>


class CommandDescription {
protected:
    const char* const name;

    // Error messages used by derived classes. Kept here to ensure one instance.
    static Sth2Opt2List error_missing, error_unexpected;
    static Sth2Opt2List error_not_string, error_not_string_null;
    static Sth2Opt2List error_unknown, error_pairless;

    // Helper methods for things that keep popping up in derived classes.
    OutputItem* array_id_name(OutputItem* Writer) const;
    bool error(Sth2Opt2List& Msg, Output* Out, const SimpleValue& Id,
        const std::vector<std::shared_ptr<SimpleValue>>& Arguments,
        size_t PastLastIndex);
    bool error(Sth2Opt2List& Msg, Output& Out, const SimpleValue& Id,
        const std::vector<std::shared_ptr<SimpleValue>>& Arguments,
        size_t PastLastIndex);
    std::string angle(const char* const Str) const;

    virtual bool validate(Output& Out, const SimpleValue& Id,
        std::vector<std::shared_ptr<SimpleValue>>& Arguments) = 0;
public:
    CommandDescription(const char* const Name);
    virtual ~CommandDescription();
    virtual void Report(OutputItem* Writer) const = 0;
    const char* const Name() const { return name; }
    // Sends done message if validation fails.
    bool Validate(Output& Out, const SimpleValue& Id,
        std::vector<std::shared_ptr<SimpleValue>>& Arguments);
};


#endif /* CommandDescription_hpp */
