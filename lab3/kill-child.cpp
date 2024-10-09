#include <iostream>
#include <signal.h>
#include <unistd.h>
#include <string>
#include <vector>

int main()
{
	pid_t pid = getpid();
	std::string command;
	std::vector<pid_t> childPids;

	while (command != "exit" && pid)
	{
		std::cin >> command;
		if (command == "spawn")
		{
			pid = fork();
		}
		else if (command == "kill")
		{
			kill(childPids.back(), SIGTERM);
		}
	}
}
