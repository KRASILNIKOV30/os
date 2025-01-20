#pragma once
#include <vector>
#include <set>
#include <map>

using namespace std;

template<typename T>
class DFA
{
public:
	explicit DFA(int initial, bool isfinal);
	~DFA(void);
	void AddState(int s, bool isfinal);
	void AddTransition(int src, T input, int dest);
	bool IsAccepting();
	bool IsAccepting(T inp);
	void Reset();
	int Input(T inp);
	int State();


private:
	int m_initial;
	int m_state;
	set<int> m_states;
	set<int> m_final_states;
	map<pair<int, T>, int> m_transitions;
};