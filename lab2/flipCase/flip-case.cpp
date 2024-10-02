#include <vector>
#include <string>
#include <iostream>
#include <optional>
#include <unistd.h>
#include <sys/wait.h>
#include <fstream>
#include <ranges>
#include <bits/ranges_algo.h>


struct Args
{
    std::vector<std::string> files;
};

std::optional<Args> ParseArgs(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cout << "Invalid argument count\n"
                  << "Usage: flip-case.exe <input1.txt> [<input2.txt> ...]]" << std::endl;
        return std::nullopt;
    }
    Args args;
    for (size_t i = 1; i <= argc - 1; i++)
    {
        args.files.emplace_back(argv[i]);
    }

    return args;
}

char FlipCase(const char ch)
{
	if (std::isupper(ch))
	{
		return tolower(ch);
	}
	if (std::islower(ch))
	{
		return toupper(ch);
	}

	return ch;
}

void FlipCaseFile(std::ifstream& input, std::ofstream& output)
{
	char ch;
	while (input.get(ch))
	{
		if (!output.put(FlipCase(ch)))
		{
			throw std::runtime_error("Failed to save data on disk");
		}
	}
}

void HandleChildProcess(std::string const& fileName)
{
	const auto selfPid = getpid();
	std::cout << "Process " << selfPid << " is processing " << fileName << std::endl;
	auto input = std::ifstream(fileName);
	auto output = std::ofstream(fileName + ".out");
	FlipCaseFile(input, output);
	std::cout << "Process " << selfPid << " has finished writing to " << fileName << ".out" << std::endl;
}

void WaitChildProcesses(std::vector<pid_t>& childPids)
{
	// сделать через while (Исправлено)
	while (!empty(childPids))
	{
		auto childPid = waitpid(-1, nullptr, 0);
		std::erase(childPids, childPid);
		if (childPid == -1)
		{
			std::cout << "waitpid failed" << std::endl;
			return;
		}
		std::cout << "Child process " << childPid << " is over" << std::endl;
	}
}
// узнать про zombie
int main(const int argc, char* argv[])
{
	const auto args = ParseArgs(argc, argv);
    if (!args)
    {
        return EXIT_FAILURE;
    }

	auto files = args->files;
	pid_t pid = getpid();
	std::vector<pid_t> childPids;
	for (auto fileIt = files.begin(); fileIt != files.end() && pid != 0; ++fileIt)
	{
		pid = fork();
		childPids.emplace_back(pid);
		if (pid == -1)
		{
			std::cout << "fork failed" << std::endl;
			return EXIT_FAILURE;
		}
		if (pid == 0)
		{
			HandleChildProcess(*fileIt);
		}
	}

	if (pid != 0)
	{
		WaitChildProcesses(childPids);
	}

    return EXIT_SUCCESS;
}
