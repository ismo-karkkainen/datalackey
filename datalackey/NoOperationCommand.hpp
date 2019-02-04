//
//  NoOperationCommand.hpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 21.11.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//
// Licensed under Universal Permissive License. See License.txt.

#ifndef NoOperationCommand_hpp
#define NoOperationCommand_hpp

#include "Command.hpp"
#include "Message.hpp"
#include "CommandDescriptions.hpp"


class NoOperationCommand : public Command {
private:
    class NoOpMessage : public Message {
    private:
        const char* const name;
    public:
        NoOpMessage(const char* const Name);
        void Report(Output& Out) const;
        void Send(Output& Out, const SimpleValue& Id) const;
    };

    NoOpMessage reply;
    ParameterlessDescription description;

public:
    NoOperationCommand(const char *const Name, Output& Out);
    ~NoOperationCommand();

    void Perform(const SimpleValue& Id,
        std::vector<std::shared_ptr<SimpleValue>>& Arguments);
};

#endif /* NoOperationCommand_hpp */
