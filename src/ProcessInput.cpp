//
//  ProcessInput.hpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 10.1.18.
//  Copyright © 2018 Ismo Kärkkäinen. All rights reserved.
//
// Licensed under Universal Permissive License. See License.txt.

#include "ProcessInput.hpp"
#include "RawDataOwner.hpp"
#include "StringValue.hpp"
#include "NumberValue.hpp"
#include "NullValue.hpp"
#include "Value_t.hpp"
#include "Number_t.hpp"
#include <cassert>


ProcessInput::ProcessInput(
    std::shared_ptr<SimpleValue>& Label, std::shared_ptr<SimpleValue>& Name)
    : label(Label), plabel(dynamic_cast<StringValue*>(Label.get())),
    name(Name), pname(dynamic_cast<StringValue*>(Name.get()))
{
    assert(plabel != nullptr);
    assert(pname != nullptr);
}

ProcessInput::ProcessInput(std::shared_ptr<SimpleValue>& Value, Encoder* E,
    std::shared_ptr<SimpleValue>& Name)
    : label(nullptr), plabel(nullptr),
    name(Name), pname(dynamic_cast<StringValue*>(Name.get()))
{
    assert(pname != nullptr);
    Encoder* enc = E->Clone();
    RawDataOwner* rdo = new RawDataOwner();
    if (IsStringValue(Value.get()))
        enc->Encode(*rdo->Raw(), ValueRef<std::string>(Value->String()));
    else {
        if (IsNumberValue(Value.get()))
            enc->Encode(*rdo->Raw(), NumberRef<long long int>(Value->Number()));
        else {
            if (IsNullValue(Value.get()))
                enc->Encode(*rdo->Raw(), Null);
            else
                assert(false);
        }
    }
    rdo->Finish();
    delete enc;
    std::shared_ptr<DataOwner> shared(rdo);
    SetData(rdo->Reader(shared));
}

ProcessInput::ProcessInput() : plabel(nullptr), pname(nullptr) { }
