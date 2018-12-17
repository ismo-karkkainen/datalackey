//
//  Processes.cpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 18.10.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#include "Processes.hpp"
#include "ProcessMessages.hpp"
#include "OutputCollection.hpp"


Processes::~Processes() { }

void Processes::NotifyStart(const SimpleValue& Id, pid_t PID, Output* Out) const
{
    if (Out == nullptr)
        ProcessNotifiedOutputs.Notify([&Id,PID](Output* Out) {
            pm_process_started.Send(*Out, Id, PID);
        });
    else
        pm_process_started.Send(*Out, Id, PID);
}

void Processes::NotifyEnd(const SimpleValue& Id, pid_t PID, Output* Out) const
{
    if (Out == nullptr)
        ProcessNotifiedOutputs.Notify([&Id,PID](Output* Out) {
            pm_process_ended.Send(*Out, Id, PID);
        });
    else
        pm_process_ended.Send(*Out, Id, PID);
}
