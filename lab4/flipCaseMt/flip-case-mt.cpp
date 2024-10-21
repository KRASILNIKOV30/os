#include <vector>
#include <string>
#include <iostream>
#include <optional>
#include <unistd.h>
#include <sys/wait.h>
#include <fstream>
#include <thread>

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
	const auto castedChar = static_cast<unsigned char>(ch);
	if (std::isupper(castedChar))
	{
		return tolower(castedChar);
	}
	if (std::islower(castedChar))
	{
		return toupper(castedChar);
	}

	return static_cast<char>(castedChar);
}

void FlipCase(std::ifstream& input, std::ofstream& output)
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

void FlipCaseFile(std::string const& fileName)
{
	auto input = std::ifstream(fileName);
	auto output = std::ofstream(fileName + ".out");
	FlipCase(input, output);
}

int main(const int argc, char* argv[])
{
	const auto args = ParseArgs(argc, argv);
	if (!args)
	{
		return EXIT_FAILURE;
	}

	auto files = args->files;
	std::vector<std::jthread> treads;
	for (auto fileIt = files.begin(); fileIt != files.end(); ++fileIt)
	{
		treads.emplace_back(FlipCaseFile, *fileIt);
	}

	return EXIT_SUCCESS;
}
