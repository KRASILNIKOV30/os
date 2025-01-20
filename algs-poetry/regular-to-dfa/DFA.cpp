#include "DFA.h"
#include "RegexTree.h"

#include <algorithm>
#include <cstddef>
#include <fstream>
#include <memory>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>
#include <vector>

namespace
{
    using namespace std;
    struct DState
    {
        DState(unique_ptr<DFA::State> sp,
            const unordered_set<size_t>& fp)
            : state(move(sp)), followpos(fp) {}
        DState(unique_ptr<DFA::State> sp, unordered_set<size_t>&& fp)
            : state(move(sp)), followpos(move(fp))
        {}

        unique_ptr<DFA::State> state;
        unordered_set<size_t> followpos;
    };
}

DFA::DFA(const RegexTree& tree)
{
    vector<DState> dstates;

    dstates.emplace_back(make_unique<State>(), tree.FirstPosRoot());

    auto& alphabet = tree.Alphabet();

    for (size_t i = 0; i < dstates.size(); ++i)
    {
        if (any_of(dstates[i].followpos.cbegin(), dstates[i].followpos.cend(),
            [endpos = tree.EndPos()](auto nextPos)
            {
                return nextPos == endpos;
            }))
        {
            dstates[i].state->MakeAcceptState();
        }

        for (auto character : alphabet)
        {
            unordered_set<size_t> newNextPositions;

            for (auto nextPos : dstates[i].followpos)
            {
                if (tree.CharAtPos(character, nextPos))
                {
                    auto followpos = tree.FollowPos(nextPos);
                    newNextPositions.insert(followpos.cbegin(), followpos.cend());
                }
            }

            if (newNextPositions.empty()) continue;

            auto oldState = find_if(
                dstates.cbegin(), dstates.cend(),
                [&nnp = as_const(newNextPositions)](const auto& dstate)
                {
                    return nnp == dstate.followpos;
                });

            if (oldState == dstates.end())
            {
                dstates.emplace_back(make_unique<State>(),
                    move(newNextPositions));
                dstates[i].state->AddTransition(dstates.back().state.get(), character);
            }
            else
            {
                dstates[i].state->AddTransition(oldState->state.get(), character);
            }
        }
    }

    for (auto& dstate : dstates) states.emplace_back(move(dstate.state));
}

void DFA::CreateDotFile(string_view fileName) const
{
    ofstream out(fileName.data());
    out << "digraph finite_state_machine {" << '\n'
        << "  rankdir=LR;" << '\n'
        << "  size=\"8,5\"" << '\n'
        << "  node [shape = circle]; q0" << '\n';

    unordered_map<const State*, size_t> stateId;
    for (size_t i = 0; i < states.size(); ++i)
    {
        auto p = states[i].get();
        for (const auto& trans : p->Transitions())
        {
            stateId.try_emplace(p, stateId.size());
            stateId.try_emplace(trans.state, stateId.size());
            auto startState = "q" + to_string(stateId[p]);
            auto endState = "q" + to_string(stateId[trans.state]);
            auto edgeLabel = trans.character;

            if (trans.state->IsAcceptState())
            {
                out << "node [shape = doublecircle];" << '\n';
            }
            else
            {
                out << "node [shape = circle];" << '\n';
            }
            out << "  " << startState << " -> " << endState << " [ label = \""
                << edgeLabel << "\" ];" << '\n';
        }
    }
    out << "}" << '\n';

    string command = "dot -Tpng _dfa-data.dot -o dfa.png";
    system(command.c_str());
}