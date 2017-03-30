//
//  StdOutErr.hpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 29.3.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#ifndef StdOutErr_hpp
#define StdOutErr_hpp

#include "MessageOutput.hpp"

// Should look like a stream, but there are message type codes and the
// rest. The rest should be accumulated to string (stringstream) and the
// code and message pairs output or stored or something.
// Let's presume that the output useful to a calling program is also
// useful to user. Essentially the same output but different format
// and transfer.
class StdOutErr : public MessageOutput {

    bool out;
public:
    ~StdOutErr();

    StdOutErr& operator<<(const std::string& s);
    StdOutErr& operator<<(const char* c);
    StdOutErr& operator<<(MessageType type);
    StdOutErr& Flush();
};


#endif /* StdOutErr_hpp */
