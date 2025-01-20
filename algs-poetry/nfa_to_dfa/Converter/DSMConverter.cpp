#include "DSMConverter.h"
#include "../Common/Lettering.h"
#include "EmptyStateRemover/EmptyStateRemover.h"
#include <set>
#include <unordered_map>
#include <iostream>

namespace
{
    struct DeterminationState
    {
        std::set<std::string> states;
        bool isFinal = false;

        bool operator==(const DeterminationState& machineState) const
        {
            return this->states == machineState.states && this->isFinal == machineState.isFinal;
        }
    };

    struct DeterminationMachine
    {
        std::vector<std::string> inputData;
        std::vector<DeterminationState> states;
        std::vector<std::vector<dev::MachineTransitionState>> machineStates;
    };

    void LogNewStates(const std::unordered_map<std::string, DeterminationState>& states)
    {
        for (const auto& state : states)
        {
            std::cout << state.first << " = ";

            for (const auto& compoundState : state.second.states)
            {
                std::cout << compoundState << " ";
            }

            std::cout << std::endl;
        }
    }

    bool DefineIsFinalState(const std::set<std::string>& newStates, const dev::Machine& originMachine)
    {
        for (const auto& newState : newStates)
        {
            auto it = std::find_if(originMachine.states.begin(), originMachine.states.end(), [newState](const dev::MachineState& element){
                return element.state == newState;
            });

            if (it->isFinal)
            {
                return true;
            }
        }

        return false;
    }

    DeterminationState DefineDeterminationState(const NonEmptyTransitions& nonEmptyTransitions, const std::set<std::string>& machineStates, const dev::Machine& originMachine)
    {
        DeterminationState newState;

        for(const auto& states : machineStates)
        {
            auto it = nonEmptyTransitions.find(states);
            newState.states.insert(it->second.begin(), it->second.end());
        }

        newState.isFinal = DefineIsFinalState(newState.states, originMachine);

        return newState;
    }

    void InitClientMachine(const DeterminationMachine& originMachine, client::Machine& newMachine)
    {
        newMachine.inputData = originMachine.inputData;
        for (auto indexI = 0; indexI < originMachine.inputData.size(); indexI++)
        {
            newMachine.machineStates.emplace_back();

            for (auto indexJ = 0; indexJ < originMachine.states.size(); indexJ++)
            {
                newMachine.machineStates.at(indexI).emplace_back();
            }
        }

        for (auto indexI = 0; indexI < originMachine.states.size(); indexI++)
        {
            newMachine.states.push_back({});
        }
    }

    client::Machine ConvertDeterminationMachine(const DeterminationMachine& machine, const std::vector<DeterminationState>& states)
    {
        client::Machine newMachine;
        InitClientMachine(machine, newMachine);

        std::unordered_map<std::string, DeterminationState> newStatesNameMap;
        for (auto index = 0; index < states.size(); index++)
        {
            newStatesNameMap.emplace(NEW_STATE_LETTER + std::to_string(index), states[index]);
        }
        LogNewStates(newStatesNameMap);

        for (auto indexI = 0; indexI < machine.states.size(); indexI++)
        {
            auto it = std::find_if(newStatesNameMap.begin(), newStatesNameMap.end(), [&](const std::pair<std::string, DeterminationState>& element){
                return element.second == machine.states.at(indexI);
            });

            if (it != newStatesNameMap.end())
            {
                newMachine.states.at(indexI).isFinal = it->second.isFinal;
                newMachine.states.at(indexI).state = it->first;
            }
        }

        for (auto indexI = 0; indexI < machine.inputData.size(); indexI++)
        {
            for (auto indexJ = 0; indexJ < machine.states.size(); indexJ++)
            {
                auto it = std::find_if(newStatesNameMap.begin(), newStatesNameMap.end(), [&](const std::pair<std::string, DeterminationState>& element){
                    return element.second.states == machine.machineStates.at(indexI).at(indexJ).states;
                });

                if (it != newStatesNameMap.end())
                {
                    newMachine.machineStates.at(indexI).at(indexJ) = it->first;
                }
            }
        }

        return newMachine;
    }

    void AddNewStateToMachine(DeterminationMachine& machine, const DeterminationState& newState)
    {
        auto it = std::find(machine.states.begin(), machine.states.end(), newState);

        if (it == machine.states.end())
        {
            machine.states.emplace_back(newState);
            for (auto& transitionStates: machine.machineStates)
            {
                if (transitionStates.size() != machine.states.size())
                {
                    transitionStates.emplace_back(dev::MachineTransitionState{DeterminationState().states});
                }
            }
        }
    }

    void MergeTransition(dev::MachineTransitionState& destinationState, const dev::MachineTransitionState& stateToMerge)
    {
        auto temp = stateToMerge.states;
        destinationState.states.merge(temp);
    }

    void DetermineState(const NonEmptyTransitions& nonEmptyTransitions, DeterminationMachine& newMachine, const dev::Machine& originMachine, const DeterminationState& processingStates)
    {
        for (const auto& state : processingStates.states)
        {
            auto it = std::find_if(originMachine.states.begin(), originMachine.states.end(), [state](const dev::MachineState& element){
                return element.state == state;
            });
            size_t stateIndex = std::distance(originMachine.states.begin(), it);

            for (auto inputDataIndex = 0; inputDataIndex < newMachine.inputData.size(); inputDataIndex++)
            {
                auto transitionCell = DefineDeterminationState(nonEmptyTransitions, originMachine.machineStates.at(inputDataIndex).at(stateIndex).states, originMachine);
                MergeTransition(newMachine.machineStates.at(inputDataIndex).at(newMachine.states.size() - 1), {transitionCell.states});
            }
        }
    }

    void FillNewStatesToDetermination(const NonEmptyTransitions& nonEmptyTransitions, const DeterminationMachine& newMachine, std::vector<DeterminationState>& statesToDetermination, const dev::Machine& originMachine)
    {
        for (const auto& transitions : newMachine.machineStates)
        {
            auto newStates = transitions.at(newMachine.states.size() - 1).states;
			const auto isFinal = DefineIsFinalState(newStates, originMachine);
            if (!newStates.empty() && std::find(statesToDetermination.begin(), statesToDetermination.end(), DeterminationState{newStates, isFinal}) == statesToDetermination.end())
            {
                statesToDetermination.push_back(DefineDeterminationState(nonEmptyTransitions, newStates, originMachine));
            }
        }
    }

    void InitDeterminationMachine(DeterminationMachine& newMachine, const dev::Machine& originMachine)
    {
        newMachine.inputData = originMachine.inputData;
        auto it = std::find(newMachine.inputData.begin(), newMachine.inputData.end(), EMPTY_INPUT_DATA);
        if (it != newMachine.inputData.end())
        {
            newMachine.inputData.erase(it);
        }

        for (auto index = 0; index < newMachine.inputData.size(); index++)
        {
            newMachine.machineStates.emplace_back();
        }
    }
}

client::Machine DSMConverter::ConvertToDSM(const dev::Machine& originMachine)
{
    m_notEmptyTransitions = EmptyStateRemover::GenerateNonEmptyTransitions(originMachine);

    DeterminationMachine newMachine;
    InitDeterminationMachine(newMachine, originMachine);

    std::vector<DeterminationState> statesToDetermination;

    statesToDetermination.push_back(DefineDeterminationState(m_notEmptyTransitions, {originMachine.states.at(0).state}, originMachine));

    for (auto index = 0; index < statesToDetermination.size(); index++)
    {
        auto processingStates = statesToDetermination[index];
        AddNewStateToMachine(newMachine, processingStates);
        DetermineState(m_notEmptyTransitions, newMachine, originMachine, processingStates);

        FillNewStatesToDetermination(m_notEmptyTransitions, newMachine, statesToDetermination, originMachine);
    }

    return ConvertDeterminationMachine(newMachine, statesToDetermination);
}
