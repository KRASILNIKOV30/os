#include "GrammarParser.h"
#include "../Common/Utils.h"

using namespace dev;

namespace
{
    struct Transition
    {
        std::string state;
        std::string inputData;
    };

    void FillTransitions(std::vector<Transition>& transitions, std::istream& input, bool isLeftSideGrammar)
    {
        std::string transition;

        while (std::getline(input, transition, '|'))
        {
            std::string state;
            std::string inputData;
            auto index = 0;

            transition = trim(transition);
            if (transition.size() == 1)
            {
                transitions.emplace_back(Transition{"", transition});

                continue;
            }

            if (isLeftSideGrammar)
            {
                state = transition.substr(0, 1);
                inputData = transition.substr(1, transition.length() - 1);
            }
            else
            {
                state = transition.substr(1, transition.length() - 1);
                inputData = transition.substr(0, 1);
            }

            transitions.emplace_back(Transition{state, inputData});
        }
    }

    void AddNewStateToMachine(Machine& machine, const MachineState& state)
    {
        auto it = std::find_if(machine.states.begin(), machine.states.end(), [state](const MachineState& element){ return element.state == state.state; });

        if (it == machine.states.end())
        {
            machine.states.emplace_back(state);
            for (auto& transitionStates: machine.machineStates)
            {
                if (transitionStates.size() != machine.states.size())
                {
                    transitionStates.emplace_back(MachineTransitionState{std::set<std::string>()});
                }
            }
        }
    }

    void AddNewInputDataToMachine(Machine& machine, const std::string& inputData)
    {
        auto it = std::find(machine.inputData.begin(), machine.inputData.end(), inputData);

        if (it == machine.inputData.end())
        {
            machine.inputData.emplace_back(inputData);
            machine.machineStates.emplace_back(std::vector<MachineTransitionState>(machine.states.size(), MachineTransitionState{std::set<std::string>()}));
        }
    }

    void FillMachineByState(Machine& machine, const std::string& state,  const std::vector<Transition>& transitions, bool isLeftSideGrammar)
    {
        bool isFinal = isLeftSideGrammar && machine.states.empty();

        AddNewStateToMachine(machine, {state, isFinal});

        size_t inputDataIndex = 0;
        size_t stateIndex = std::distance(machine.states.begin(), std::find(machine.states.begin(), machine.states.end(), MachineState{state, isFinal}));
        isFinal = false;

        for (const auto& transition : transitions)
        {
            std::string inputData = transition.inputData;
            std::string newState = transition.state.empty() ? DEFAULT_UNNAMED_STATE : transition.state;

            if (!isLeftSideGrammar)
            {
                isFinal = newState == DEFAULT_UNNAMED_STATE;
            }

            AddNewInputDataToMachine(machine, inputData);
            AddNewStateToMachine(machine, {newState, isFinal});
            inputDataIndex = std::distance(machine.inputData.begin(), std::find(machine.inputData.begin(), machine.inputData.end(), inputData));

            if (isLeftSideGrammar)
            {
                stateIndex = std::distance(machine.states.begin(), std::find_if(machine.states.begin(), machine.states.end(),
                                                                                [newState](const MachineState& element){ return element.state == newState; }));
                newState = state;
            }

            try
            {
                machine.machineStates.at(inputDataIndex).at(stateIndex).states.emplace(newState);
            }
            catch (...)
            {
                machine.machineStates.at(inputDataIndex).emplace_back(MachineTransitionState{std::set<std::string>{newState}});
            }
        }
    }
}

Machine GrammarParser::ParseGrammarToMachine(std::istream& input, GrammarSide grammarSide)
{
    Machine machine;
    grammarSide == GrammarSide::RIGHT
        ? ParseGrammarToMachine(machine, input, false)
        : ParseGrammarToMachine(machine, input, true);

    return machine;
}

void GrammarParser::ParseGrammarToMachine(Machine& machine, std::istream& input, bool isLeftSideGrammar)
{
    std::string line;
    while (std::getline(input, line))
    {
        std::istringstream iss(line);
        std::string state;
        std::string trash;
        iss >> state;

        iss >> trash;

        std::vector<Transition> transitions;
        FillTransitions(transitions, iss, isLeftSideGrammar);
        FillMachineByState(machine, state, transitions, isLeftSideGrammar);
    }
}