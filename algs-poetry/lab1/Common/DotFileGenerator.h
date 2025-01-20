#pragma once

#include "../Machine/Machine.h"

inline void GenerateDotFile(const Machine& machine, const std::string& fileName)
{
	std::ofstream outputFile(fileName);
	outputFile << "digraph machine {" << std::endl;

	for (const auto& state : machine.states)
	{
		outputFile << state << " [label = \"" << state << "\"]" << std::endl;
	}
	outputFile << std::endl;

	for (int i = 0; i < machine.states.size(); i++)
	{
		for (int j = 0; j < machine.states[i].size(); j++)
		{
			std::string outputData = machine.machineStates[j][i].outputData.has_value() ? "/" + machine.machineStates[j][i].outputData.value() : "";
			outputFile << machine.states[i] << " -> " << machine.machineStates[j][i].state << " [label = \"" << machine.inputData[j] << outputData << "\"];" << std::endl;
		}
	}

	outputFile << "}" << std::endl;
}
