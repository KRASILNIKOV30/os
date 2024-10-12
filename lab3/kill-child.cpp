#include <iostream>
#include <csignal>
#include <unistd.h>
#include <string>
#include <vector>
#include <atomic>
#include <chrono>
#include <thread>
#include <sys/wait.h>

std::atomic_flag exitRequested;

void SignalHandler(const int signum)
{
	if (signum == SIGTERM)
	{
		exitRequested.test_and_set();
	}
}

// Вынести в класс Child и, возможно, Manager
// Убрать глобальную переменную
void PrintAliveMessages()
{
	while (!exitRequested.test())
	{
		std::this_thread::sleep_for(std::chrono::seconds(5));
		if (exitRequested.test())
		{
			return;
		}

		std::cout << "Child process " << getpid() << " is still alive" << std::endl;
	}
}

void ChildProcessAction()
{
	if (std::signal(SIGTERM, SignalHandler) == SIG_ERR)
	{
		std::cerr << "std::signal failed" << std::endl;
	}
	PrintAliveMessages();
}

bool KillChild(std::vector<pid_t>& childPids)
{
	if (childPids.empty())
	{
		return false;
	}
	const auto pidToKill = childPids.back();
	if (kill(pidToKill, SIGTERM) != 0)
	{
		std::cerr << "kill failed" << std::endl;
	}
	if (waitpid(pidToKill, nullptr, 0) != -1)
	{
		std::cerr << "Child process " << pidToKill << " was killed" << std::endl;
		childPids.pop_back();
	}

	return true;
}

int main()
{
	pid_t pid = getpid();
	std::string command;
	//list
	//Можно добавить в деструкторе убийство всех детей
	std::vector<pid_t> childPids;

	while (command != "exit" && pid)
	{
		std::cin >> command;
		if (command == "spawn")
		{
			pid = fork();
			if (pid != 0)
			{
				std::cout << "Child process " << pid << " have been spawned" << std::endl;
				childPids.push_back(pid);
			}
		}
		else if (command == "kill")
		{
			if (!KillChild(childPids))
			{
				std::cerr << "No children to kill" << std::endl;
			}
		}
	}

	if (pid == 0)
	{
		ChildProcessAction();
	}

	return EXIT_SUCCESS;
}
