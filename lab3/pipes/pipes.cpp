#include "Fetcher.h"
#include <iostream>
#include <sstream>
#include <vector>

void HandleAddCommand(Fetcher const& fetcher)
{
	std::string argsStr;
	std::getline(std::cin, argsStr);
	std::istringstream str(argsStr);
	std::vector<int> args;
	int arg;
	while (str >> arg)
	{
		args.push_back(arg);
	}

	std::cout << "sum is " << fetcher.FetchSumCalculation(args) << std::endl;
}

void HandleLongestWordCommand(Fetcher const& fetcher)
{
	std::string fileName;
	std::cin >> fileName;

	std::cout << "longest word is " << fetcher.FetchLongestWordSearching(fileName) << std::endl;
}

int main()
{
	ProcessManager pipedCommandHandler{ SocketPair() };
	if (pipedCommandHandler.GetPid() == 0)
	{
		return EXIT_SUCCESS;
	}
	const Fetcher fetcher(std::move(pipedCommandHandler));

	std::string command;
	while (true)
	{
		std::cin >> command;
		if (command == "add")
		{
			HandleAddCommand(fetcher);
		}
		else if (command == "longest_word")
		{
			HandleLongestWordCommand(fetcher);
		}
		if (command == "exit")
		{
			break;
		}
	}

	return EXIT_SUCCESS;
}

