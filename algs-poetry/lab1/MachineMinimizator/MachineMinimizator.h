#pragma once

#include "../Machine/Machine.h"

namespace
{
    struct MachineWithEquivalentStates;
} //namespace

class MachineMinimizator
{
public:
    static Machine MinimizeMoore(const Machine& machine);

    static Machine MinimizeMealy(const Machine& machine);

private:
    static Machine MinimizeMachine(MachineWithEquivalentStates& machine, const Machine& originMachine);

    static void SetTransitionWithNewEquivalentStates(MachineWithEquivalentStates& machine, const Machine& originMachine);
    static Machine CreateNewMachineByEquivalentStates(MachineWithEquivalentStates& machine, const Machine& originMachine);
};