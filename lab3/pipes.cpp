#include <iostream>
#include <stdexcept>
#include <system_error>
#include <unistd.h>
#include <utility>
#include <vector>

void HandleAddCommand()
{
}

void ParseAddCommand()
{
	std::string paramsLine;
	std::getline(std::cin, paramsLine);
	int param;
	std::vector<int> params;
	while (std::cin >> param)
	{
		params.emplace_back(param);
	}

	int myPipe[2];
	if (pipe(myPipe) != 0)
	{
		throw std::system_error(errno, std::generic_category());
	}

	pid_t pid = fork();
	if (pid == 0)
	{
		HandleAddCommand();
	}

}

int main()
{
	std::string command;
	while (true)
	{
		std::cin >> command;
		if (command == "add")
		{

		}
		else if (command == "longest_word")
		{

		}
		if (command == "exit")
		{
			break;
		}
	}
}

