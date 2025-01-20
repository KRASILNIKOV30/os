#include "MachineMinimizator.h"

#include "../Common/Lettering.h"
#include "map"
#include <set>
#include <stdexcept>

namespace
{
using StateToTransition = std::map<std::string, std::string>;

struct MachineWithEquivalentStates : Machine
{
	StateToTransition equivalentStates;
};

StateToTransition CreateNewEquivalentStates(const StateToTransition& transitionsForEquivalentStates, const StateToTransition& oldEquivalentStates)
{
	auto newEquivalentStates = transitionsForEquivalentStates;
	const std::string newStateName = { GetNewLetter(newEquivalentStates.begin()->second.at(0)) };

	auto index = 0;
	for (auto transition = newEquivalentStates.begin(); transition != newEquivalentStates.end(); ++transition)
	{
		if (transition->second.at(0) == newStateName.at(0))
		{
			continue;
		}

		auto replacementTransition = transition->second;
		transition->second = newStateName + std::to_string(index);
		index++;

		auto equivalentStateOfInitiator = oldEquivalentStates.find(transition->first);

		for (auto sameTransition = transition; sameTransition != newEquivalentStates.end(); sameTransition++)
		{
			auto equivalentStateOfCurrent = oldEquivalentStates.find(sameTransition->first);

			if (sameTransition->second == replacementTransition && (equivalentStateOfInitiator == oldEquivalentStates.end() || equivalentStateOfInitiator->second == equivalentStateOfCurrent->second))
			{
				sameTransition->second = transition->second;
			}
		}
	}

	return newEquivalentStates;
}

StateToTransition GetTransitionsToStates(const MachineWithEquivalentStates& machine)
{
	StateToTransition transitionsForEquivalentStates;
	const size_t inputsCount = machine.inputData.size();
	const size_t statesCount = machine.states.size();

	for (auto indexI = 0; indexI < statesCount; indexI++)
	{
		std::string states;
		for (auto indexJ = 0; indexJ < inputsCount; indexJ++)
		{
			states += machine.machineStates.at(indexJ).at(indexI).state;
		}
		transitionsForEquivalentStates.emplace(std::pair<std::string, std::string>{ machine.states.at(indexI), states });
	}

	return transitionsForEquivalentStates;
}

std::set<std::string> GetEquivalentStates(const MachineWithEquivalentStates& machine)
{
	std::set<std::string> equivalentStates;
	for (const auto& equivalentState : machine.equivalentStates)
	{
		equivalentStates.insert(equivalentState.second);
	}

	return equivalentStates;
}
} // namespace

Machine MachineMinimizator::MinimizeMealy(const Machine& machine)
{
	MachineWithEquivalentStates newMachine = { machine.inputData, machine.states, machine.machineStates, machine.outputData };
	StateToTransition transitionsForEquivalentStates;
	const size_t inputsCount = newMachine.inputData.size();
	const size_t statesCount = newMachine.states.size();

	for (auto indexI = 0; indexI < statesCount; indexI++)
	{
		std::string states;
		for (auto indexJ = 0; indexJ < inputsCount; indexJ++)
		{
			states += *machine.machineStates.at(indexJ).at(indexI).outputData;
		}
		transitionsForEquivalentStates.emplace(std::pair<std::string, std::string>{ newMachine.states.at(indexI), states });
	}

	newMachine.equivalentStates = CreateNewEquivalentStates(transitionsForEquivalentStates, newMachine.equivalentStates);

	SetTransitionWithNewEquivalentStates(newMachine, machine);

	return MinimizeMachine(newMachine, machine);
}

Machine MachineMinimizator::MinimizeMoore(const Machine& machine)
{
	MachineWithEquivalentStates newMachine = { machine.inputData, machine.states, machine.machineStates, machine.outputData };

	StateToTransition transitionsForEquivalentStates;

	for (auto indexI = 0; indexI < machine.states.size(); indexI++)
	{
		transitionsForEquivalentStates.emplace(std::pair<std::string, std::string>{ machine.states.at(indexI), machine.outputData.at(indexI) });
	}

	newMachine.equivalentStates = CreateNewEquivalentStates(transitionsForEquivalentStates, newMachine.equivalentStates);
	SetTransitionWithNewEquivalentStates(newMachine, machine);
	return MinimizeMachine(newMachine, machine);
}

Machine MachineMinimizator::MinimizeMachine(MachineWithEquivalentStates& machine, const Machine& originMachine)
{
	size_t countOfEquivalentStates = 0;
	size_t currentCountOfEquivalentStates = GetEquivalentStates(machine).size();
	while (countOfEquivalentStates != currentCountOfEquivalentStates && currentCountOfEquivalentStates != machine.states.size())
	{
		auto transitionsForEquivalentStates = GetTransitionsToStates(machine);
		machine.equivalentStates = CreateNewEquivalentStates(transitionsForEquivalentStates, machine.equivalentStates);
		SetTransitionWithNewEquivalentStates(machine, originMachine);
		countOfEquivalentStates = currentCountOfEquivalentStates;
		currentCountOfEquivalentStates = GetEquivalentStates(machine).size();
	}

	return CreateNewMachineByEquivalentStates(machine, originMachine);
}

void MachineMinimizator::SetTransitionWithNewEquivalentStates(MachineWithEquivalentStates& machine, const Machine& originMachine)
{
	const size_t inputsCount = machine.inputData.size();
	const size_t statesCount = machine.states.size();

	for (auto indexJ = 0; indexJ < statesCount; indexJ++)
	{
		auto currentState = machine.states.at(indexJ);

		for (auto indexI = 0; indexI < inputsCount; indexI++)
		{
			auto originState = originMachine.machineStates.at(indexI).at(indexJ).state;
			auto it = machine.equivalentStates.find(originState);

			if (it == machine.equivalentStates.end())
			{
				throw std::runtime_error("");
			}

			machine.machineStates.at(indexI).at(indexJ).state = it->second;
			machine.machineStates.at(indexI).at(indexJ).outputData = originMachine.machineStates.at(indexI).at(indexJ).outputData;
		}
	}
}

Machine MachineMinimizator::CreateNewMachineByEquivalentStates(MachineWithEquivalentStates& machine, const Machine& originMachine)
{
	Machine newMachine;
	newMachine.inputData = machine.inputData;
	auto setNewStates = GetEquivalentStates(machine);
	newMachine.states = std::vector<std::string>(setNewStates.size());
	std::copy(setNewStates.begin(), setNewStates.end(), newMachine.states.begin());
	std::sort(newMachine.states.begin(), newMachine.states.end());

	const size_t inputsCount = newMachine.inputData.size();
	const size_t statesCount = newMachine.states.size();

	if (!std::empty(originMachine.outputData))
	{
		for (auto index = 0; index < statesCount; index++)
		{
			const auto it = std::find_if(machine.equivalentStates.begin(), machine.equivalentStates.end(), [=](const std::pair<std::string, std::string>& element) {
				return element.second == newMachine.states.at(index);
			});
			const auto distance = std::distance(originMachine.states.begin(), std::find(originMachine.states.begin(), originMachine.states.end(), it->first));
			newMachine.outputData.push_back(originMachine.outputData.at(distance));
		}
	}

	for (auto index = 0; index < inputsCount; index++)
	{
		newMachine.machineStates.emplace_back(statesCount, MachineState());
	}

	for (auto indexJ = 0; indexJ < statesCount; indexJ++)
	{
		const auto currentState = newMachine.states.at(indexJ);
		const auto originState = std::find_if(machine.equivalentStates.begin(), machine.equivalentStates.end(), [=](const std::pair<std::string, std::string>& element) {
			return element.second == currentState;
		});
		const auto it = std::find(originMachine.states.begin(), originMachine.states.end(), originState->first);
		const auto distance = std::distance(originMachine.states.begin(), it);

		for (auto indexI = 0; indexI < inputsCount; indexI++)
		{
			MachineState oldState = originMachine.machineStates.at(indexI).at(distance);
			const auto newState = machine.equivalentStates.find(oldState.state);

			newMachine.machineStates.at(indexI).at(indexJ).state = newState->second;
			newMachine.machineStates.at(indexI).at(indexJ).outputData = oldState.outputData;
		}
	}

	return newMachine;
}
