#pragma once

#include <fstream>
#include <optional>
#include <string>
#include <vector>

enum class MachineType
{
	Mealy,
	Moore
};

struct MachineState
{
	std::string state;
	std::optional<std::string> outputData = std::nullopt;

	bool operator==(const MachineState& machineState) const
	{
		return this->state == machineState.state && this->outputData == machineState.outputData;
	}
};

struct Machine
{
	std::vector<std::string> inputData;
	std::vector<std::string> states;
	std::vector<std::vector<MachineState>> machineStates;
	std::vector<std::string> outputData;
};

struct Transition
{
	std::string fromState;
	std::string toState;
	std::string output;
};
