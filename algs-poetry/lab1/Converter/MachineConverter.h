#pragma once

#include "../Common/Lettering.h"
#include "../Machine/Machine.h"
#include <bits/ranges_algo.h>
#include <map>

class MachineConverter
{
public:
	Machine static GetMooreMachineFromMealy(const Machine& machine)
	{
		Machine newMachine;
		const auto newStates = CreateMapStates(machine);

		SetInitialStatesAndOutputData(newMachine, newStates);
		SetInputData(newMachine, machine.inputData);
		SetTransitionData(newMachine, machine, newStates);
		return newMachine;
	}

	Machine static GetMealyMachineFromMoore(const Machine& machine)
	{
		Machine newMachine = machine;

		return newMachine;
	}

private:
	std::map<std::string, MachineState> static CreateMapStates(const Machine& machine)
	{
		const std::string newStateName = { GetNewLetter(machine.states.at(0).at(0)) };
		std::map<std::string, MachineState> transitionMap;

		for (auto const& transitionLine : machine.machineStates)
		{
			for (auto const& state : transitionLine)
			{
				if (!IsValueExist(transitionMap, state))
				{
					transitionMap.insert(std::make_pair(newStateName + std::to_string(transitionMap.size()), state));
				}
			}
		}

		return transitionMap;
	}

	bool static IsValueExist(const std::map<std::string, MachineState>& machineMap, const MachineState& state)
	{
		return std::ranges::any_of(machineMap.begin(), machineMap.end(), [state](const auto& pair) { return pair.second == state; });
	}

	void static SetInitialStatesAndOutputData(Machine& machine, const std::map<std::string, MachineState>& newStates)
	{
		for (auto const& state : newStates)
		{
			machine.states.push_back(state.first);
			machine.outputData.push_back(*state.second.outputData);
		}
	}

	void static SetInputData(Machine& machine, const std::vector<std::string>& inputData)
	{
		machine.inputData = inputData;
	}

	void static SetTransitionData(Machine& machine, const Machine& oldMachine, const std::map<std::string, MachineState>& newStates)
	{
		const size_t inputsCount = machine.inputData.size();
		const size_t statesCount = machine.states.size();
		std::vector test(1, MachineState());

		for (auto indexI = 0; indexI < inputsCount; indexI++)
		{
			machine.machineStates.emplace_back(statesCount, MachineState());
		}

		for (auto indexJ = 0; indexJ < statesCount; indexJ++)
		{
			auto stateFrom = machine.states.at(indexJ);
			const auto initialStateIt = std::ranges::find_if(newStates, [&](const std::pair<std::string, MachineState>& element) {
				return element.first == stateFrom;
			});
			const auto initialState = initialStateIt->second;

			const auto indexOfNewStates = std::distance(oldMachine.states.begin(), std::ranges::find(oldMachine.states, initialState.state));

			for (auto indexI = 0; indexI < inputsCount; indexI++)
			{
				auto oldState = oldMachine.machineStates.at(indexI).at(indexOfNewStates);

				const auto it = std::ranges::find_if(newStates, [&](const std::pair<std::string, MachineState>& element) {
					return element.second == oldState;
				});
				machine.machineStates.at(indexI).at(indexJ).state = it->first;
			}
		}
	}
};