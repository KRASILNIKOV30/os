#include "EmptyStateRemover.h"
#include "../../Common/Lettering.h"

namespace
{
    void FillNotEmptyState(const Machine& machine, std::set<std::string>& newState, const std::string& currentState)
    {
        auto it = std::find_if(machine.states.begin(), machine.states.end(), [currentState](const MachineState& element){
            return element.state == currentState;
        });

        size_t stateIndex = std::distance(machine.states.begin(), it);
        size_t inputDataIndex = std::distance(machine.inputData.begin(), std::find(machine.inputData.begin(), machine.inputData.end(), EMPTY_INPUT_DATA));

        auto transition = machine.machineStates.at(inputDataIndex).at(stateIndex).states;
        auto temp = transition;
        temp.insert(newState.begin(), newState.end());
        if (!transition.empty() && temp.size() != newState.size())
        {
            for (const auto& newTransitionState : transition)
            {
                newState = temp;
                FillNotEmptyState(machine, newState, newTransitionState);
            }
        }
    }
}

NonEmptyTransitions EmptyStateRemover::GenerateNonEmptyTransitions(const Machine& machine)
{
    NonEmptyTransitions newTransitions;

    for (const auto& machineState : machine.states)
    {
        std::set<std::string> newState;
        newState.emplace(machineState.state);
        FillNotEmptyState(machine, newState, machineState.state);
        newTransitions.emplace(machineState.state, newState);
    }

    return newTransitions;
}
