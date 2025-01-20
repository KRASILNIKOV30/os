#pragma once

#include <sstream>
#include "../Machine/Machine.h"

using namespace dev;

class CSVTextParser
{
public:

    static Machine GetMachine(std::istream& istream);

private:
    void static ProcessTypicalMachineInputData(std::istream& istream, Machine& machine);

    size_t static GetFinalStatePosition(std::istream& istream);

    void static FillMachineStateTransitions(Machine& machine, std::vector<std::string>& transitions);
};