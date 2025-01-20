#pragma once

#include <sstream>
#include "../Machine/Machine.h"

namespace
{
	constexpr auto EMPTY_OUTPUT_SYMBOL = "empty";
}

class CSVTextParser
{
public:
    static Machine GetMealy(std::istream& istream)
    {
        Machine machine = {};

        ProcessTypicalMachineInputData(istream, machine);

        return machine;
    }

    static Machine GetMoore(std::istream& istream)
    {
        Machine machine = {};

        ProcessSpecificMachineInputData(istream, machine);
        ProcessTypicalMachineInputData(istream, machine);

        return machine;
    }

private:
    void static ProcessTypicalMachineInputData(std::istream& istream, Machine& machine)
    {
        std::string line;
        std::getline(istream, line);
        std::vector<std::string> parsedLine;
        ParseLine(line, parsedLine);
        parsedLine.erase(parsedLine.begin());
        machine.states = parsedLine;

        while (std::getline(istream, line))
        {
            std::vector<std::string> values;
            ParseLine(line, values);
            machine.inputData.push_back(values.at(0));
            values.erase(values.begin());
            FillMachineStateTransitions(machine, values);
        }
    }

    void static ProcessSpecificMachineInputData(std::istream& istream, Machine& machine)
    {
        std::string line;
        std::getline(istream, line);
        std::vector<std::string> parsedLine;
        ParseLine(line, parsedLine);
        parsedLine.erase(parsedLine.begin());
        machine.outputData = parsedLine;
    }

    void static FillMachineStateTransitions(Machine& machine, std::vector<std::string>& transitions)
    {
        std::vector<MachineState> machineStates;

        for (const auto& transition : transitions)
        {
            MachineState state;
            auto index = transition.find("/");
            if (index != std::string::npos)
            {
                state.state = transition.substr(0, index);
                state.outputData = transition.substr(index + 1, transition.length());
            }
            else
            {
                auto it = std::find(machine.states.begin(), machine.states.end(), transition);
                state.state = transition;
                state.outputData = machine.outputData.at(std::distance(machine.states.begin(), it));
            }
            machineStates.push_back(state);
        }

        machine.machineStates.push_back(machineStates);
    }

    void static ParseLine(std::string& line, std::vector<std::string>& states)
    {
        std::string state;
        std::istringstream ss(line);

        while (std::getline(ss, state, ';'))
        {
			if (state.empty())
			{
				state = EMPTY_OUTPUT_SYMBOL;
			}
            states.push_back(state);
        }
    }
};