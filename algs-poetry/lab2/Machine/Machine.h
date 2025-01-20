#pragma once

#include <vector>
#include <set>
#include <string>

namespace dev
{
    struct MachineTransitionState
    {
        std::set<std::string> states;
    };

    struct MachineState
    {
        std::string state;
        bool isFinal = false;

        bool operator==(const MachineState& machineState) const
        {
            return this->state == machineState.state && this->isFinal == machineState.isFinal;
        }
    };

    struct Machine
    {
        std::vector<std::string> inputData;
        std::vector<MachineState> states;
        std::vector<std::vector<MachineTransitionState>> machineStates;
    };
}

namespace client
{
    struct MachineState
    {
        std::string state;
        bool isFinal = false;

        bool operator==(const MachineState& machineState) const
        {
            return this->state == machineState.state && this->isFinal == machineState.isFinal;
        }
    };

    struct Machine
    {
        std::vector<std::string> inputData;
        std::vector<MachineState> states;
        std::vector<std::vector<std::string>> machineStates;
    };
}

constexpr auto DEFAULT_UNNAMED_STATE = "UNNAMED";