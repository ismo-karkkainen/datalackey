//
//  MessageOutput.hpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 16.3.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#ifndef MessageOutput_hpp
#define MessageOutput_hpp

#include <sstream>

enum MessageType {
    Normal = 1,
    Warning,
    Error
};

// Should look like a stream, but there are message type codes and the
// rest. The rest should be accumulated to string (stringstream) and the
// code and message pairs output or stored or something.
// Let's presume that the output useful to a calling program is also
// useful to user. Essentially the same output but different format
// and transfer.
class MessageOutput {
    // Simple transfer is stdout/stderr based on code.
    // Then there is JSON message array accumulation.

    // Transfer classes are shared with programs to launch except output to
    // stdout and stderr is special as there are two streams. In that case the
    // derived class prints things to user directly.
    // Any other format can take output stream and one of those can use stdout
    // to output to pipe out.
    std::ostringstream converter;
public:
    virtual ~MessageOutput();

    inline std::ostringstream& Converter() { return converter; }
    virtual MessageOutput& operator<<(const std::string& s) = 0;
    virtual MessageOutput& operator<<(const char* c) = 0;
    virtual MessageOutput& operator<<(MessageType type) = 0;
    virtual MessageOutput& Flush() = 0;
};

template<typename T>
MessageOutput& operator<<(MessageOutput& out, const T& value) {
    out.Converter() << value;
    out << out.Converter().str();
    out.Converter().str(std::string());
    return out;
}

#endif /* MessageOutput_hpp */
