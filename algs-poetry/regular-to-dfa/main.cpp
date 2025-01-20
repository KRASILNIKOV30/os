#include <iostream>
#include <string>
#include "DFA.h"
#include "RegexTree.h"

namespace
{
	using namespace std;

	string ReadLine()
	{
		string str;	
		while (!str.size())
		{
			getline(cin, str);
		}
		return str;
	}
}

int main()
{
	cout << "Enter regex: ";
	auto regex = ReadLine();
	auto tree = RegexTree(regex);
	tree.PrintTree();
	auto dfa = DFA(tree);
	dfa.CreateDotFile("_dfa-data.dot");
}