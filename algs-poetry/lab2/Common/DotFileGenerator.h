#pragma once

#include "../Machine/Machine.h"

#include <fstream>
#include <ostream>

inline void GenerateDotFile(const client::Machine& machine, const std::string& fileName)
{
	std::ofstream outputFile(fileName);
	outputFile << "digraph machine {" << std::endl;

	for (const auto& [state, isFinal] : machine.states)
	{
		std::string finalLabel = isFinal ? " [shape=doublecircle]" : "";
		outputFile << state << finalLabel << " [label = \"" << state << "\"]" << std::endl;
	}
	outputFile << std::endl;

	for (size_t i = 0; i < machine.states.size(); i++)
	{
		for (size_t j = 0; j < machine.inputData.size(); j++)
		{
			const std::string& fromState = machine.states[i].state;
			const std::string& toState = machine.machineStates[j][i];
			const std::string& input = machine.inputData[j];

			if (!toState.empty())
			{
				outputFile << fromState << " -> " << toState << " [label = \"" << input << "\"];" << std::endl;
			}
		}
	}

	outputFile << "}" << std::endl;
}