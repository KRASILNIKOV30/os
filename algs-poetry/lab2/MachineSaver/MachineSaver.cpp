#include "MachineSaver.h"
#include <iostream>

namespace
{
    constexpr auto SEPARATOR = ";";
    constexpr auto FINAL_STATE_SYMBOL = "F";
    constexpr auto EMPTY_SYMBOL = "-";

    void SaveAloneStates(std::ostream& output, const std::vector<client::MachineState>& states, bool needToSkipFirstValue = false)
    {
        if (needToSkipFirstValue)
        {
            output << SEPARATOR;
        }

        for (auto index = 0; index < states.size() - 1; index++)
        {
            output << states[index].state;
            output << SEPARATOR;
        }

        output << states[states.size() - 1].state;

        output << std::endl;
    }

    void SaveStateFinalSettings(std::ostream& output, const std::vector<client::MachineState>& states, bool needToSkipFirstValue = false)
    {
        if (needToSkipFirstValue)
        {
            output << SEPARATOR;
        }

        for (auto index = 0; index < states.size() - 1; index++)
        {
            if (states[index].isFinal)
            {
                output << FINAL_STATE_SYMBOL;
            }
            output << SEPARATOR;
        }

        if (states.at(states.size() - 1).isFinal)
        {
            output << FINAL_STATE_SYMBOL;
        }

        output << std::endl;
    }

    void SaveTransitions(std::ostream& output, const client::Machine& machine)
    {
        int inputsCount = machine.inputData.size();
        int statesCount = machine.states.size();

        for (auto indexI = 0; indexI < inputsCount; indexI++)
        {
            output << machine.inputData.at(indexI);

            for (auto indexJ = 0; indexJ < statesCount; indexJ++)
            {
                output << SEPARATOR;
                auto currentState = machine.machineStates.at(indexI).at(indexJ);
                output << currentState;
                if (machine.machineStates.at(indexI).at(indexJ).empty())
                {
                    output << EMPTY_SYMBOL;
                }
            }

            output << std::endl;
        }
    }
}

void MachineSaver::Save(std::ostream& output, const client::Machine& machine)
{
    SaveStateFinalSettings(output, machine.states, true);
    SaveAloneStates(output, machine.states, true);
    SaveTransitions(output, machine);
}
