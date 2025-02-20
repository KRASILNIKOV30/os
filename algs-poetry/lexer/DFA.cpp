#include "DFA.h"

template<typename T>
DFA<T>::DFA(int initial, bool isfinal)
{
	this->m_initial = this->m_state = initial;
	AddState(initial, isfinal);
	AddState(-1, false);
}

template<typename T>
DFA<T>::~DFA(void)
{
}

template<typename T>
void DFA<T>::AddState(int s, bool isfinal)
{
	m_states.insert(s);
	if (isfinal) m_final_states.insert(s);
}

template<typename T>
void DFA<T>::AddTransition(int src, T input, int dest)
{
	m_transitions.insert(pair<pair<int, T>, int>(pair<int, T>(src, input), dest));
}

template<typename T>
void DFA<T>::Reset()
{
	m_state = m_initial;
}

template<typename T>
int DFA<T>::Input(T inp)
{
	auto tr = make_pair(m_state, inp);
	if (m_transitions.count(tr) > 0)
	{
		auto it = m_transitions.find(tr);
		return m_state = it->second;
	}
	else
	{
		return m_state = -1;
	}
}

template<typename T>
bool DFA<T>::IsAccepting()
{
	return m_final_states.count(m_state) != 0;
}

template<typename T>
bool DFA<T>::IsAccepting(T inp)
{
	auto tr = make_pair(m_state, inp);
	if (m_transitions.count(tr) > 0)
	{
		auto it = m_transitions.find(tr);
		return m_final_states.count(it->second) != 0;
	}
	else return false;
}

template<typename T>
int DFA<T>::State()
{
	return m_state;
}


template class DFA<char>;