#pragma once

#include <iostream>
#include <iterator>
#include "../Machine/Machine.h"

class MachineSaver
{
public:

    void static SaveMealyMachine(std::ostream& output, const Machine& machine)
    {
        SaveAloneStates(output, machine.states, true);
        SaveTransitions(output, machine, true);
    }

    void static SaveMooreMachine(std::ostream& output, const Machine& machine)
    {
        SaveAloneStates(output, machine.outputData, true);
        SaveAloneStates(output, machine.states, true);
        SaveTransitions(output, machine);
    }

private:

    void static SaveAloneStates(std::ostream& output, const std::vector<std::string>& states, bool needToSkipFirstValue = false)
    {
        if (needToSkipFirstValue)
        {
            output << m_separator;
        }

        std::copy(states.begin(), states.end() - 1, std::ostream_iterator<std::string>(output, m_separator));
        output << states.at(states.size() - 1) << std::endl;
    }

    void static SaveTransitions(std::ostream& output, const Machine& machine, bool needToShowOutputWithState = false)
    {
        int inputsCount = machine.inputData.size();
        int statesCount = machine.states.size();

        for (auto indexI = 0; indexI < inputsCount; indexI++)
        {
            output << machine.inputData.at(indexI);

            for (auto indexJ = 0; indexJ < statesCount; indexJ++)
            {
                output << m_separator;
                auto currentState = machine.machineStates.at(indexI).at(indexJ);
                output << currentState.state;
                if (needToShowOutputWithState)
                {
                    output << m_stateSeparator << *currentState.outputData;
                }
            }

			output << std::endl;
        }
    }

    static inline auto m_separator = ";";
    static inline auto m_stateSeparator = "/";
};