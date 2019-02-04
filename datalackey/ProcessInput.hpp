//
//  ProcessInput.hpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 10.1.18.
//  Copyright © 2018 Ismo Kärkkäinen. All rights reserved.
//
// Licensed under Universal Permissive License. See License.txt.

#ifndef ProcessInput_hpp
#define ProcessInput_hpp

#include "RawData.hpp"
#include "SimpleValue.hpp"
#include "StringValue.hpp"
#include "DataOwner.hpp"
#include "Encoder.hpp"
#include <memory>


class ProcessInput {
private:
    std::shared_ptr<SimpleValue> label;
    StringValue* plabel;
    std::shared_ptr<DataReader> data;
    std::shared_ptr<SimpleValue> name;
    StringValue* pname;

public:
    // Both are expected to be StringValues.
    ProcessInput(std::shared_ptr<SimpleValue>& Label,
        std::shared_ptr<SimpleValue>& Name);
    // Value can be anything, the name is expected to be StringValue.
    ProcessInput(std::shared_ptr<SimpleValue>& Value, Encoder* E,
        std::shared_ptr<SimpleValue>& Name);
    // Special case used as end of file marker for output feeder thread.
    ProcessInput();

    // For setting data from label.
    const StringValue* Label() const { return plabel; }
    void SetData(std::shared_ptr<DataReader> Data) { data = Data; }

    // For passing data onwards.
    const StringValue* Name() const { return pname; }
    std::shared_ptr<DataReader> Data() { return data; }

    // Needed in case of errors.
    std::shared_ptr<SimpleValue>& SharedLabel() { return label; }
};


#endif /* ProcessInput_hpp */
