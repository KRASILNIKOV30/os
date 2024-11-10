#include <string>
#include <iostream>
#include <optional>
#include <unistd.h>
#include <fstream>
#include "Generator.h"
#include "Sorter.h"
#include "Timer.h"
#include <cassert>
#include <bits/fs_fwd.h>
#include <bits/fs_path.h>

namespace fs = std::filesystem;

struct GenerateArgs
{
	std::string outputFileName;
	size_t size;
};

std::optional<GenerateArgs> ParseGenerateArgs(int const argc, char* argv[])
{
	if (argc != 4)
	{
		throw std::invalid_argument("Invalid argument count\n Usage: sorter sort OUTPUT_FILE SIZE\n");
	}
	GenerateArgs args;
	args.outputFileName = argv[2];
	try
	{
		args.size = std::stoi(argv[3]);
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}

	return args;
}

struct SortArgs
{
	std::string inputFileName;
	std::string outputFileName;
	unsigned threadsNum;
};

std::optional<SortArgs> ParseSortArgs(int const argc, char* argv[])
{
	if (argc != 5)
	{
		throw std::invalid_argument("Invalid argument count\n Usage: sorter sort INPUT_FILE OUTPUT_FILE NUM_THREADS\n");
	}
	SortArgs args;
	args.inputFileName = argv[2];
	args.outputFileName = argv[3];
	try
	{
		args.threadsNum = std::stoi(argv[4]);
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}

	return args;
}

void Generate(const int argc, char* argv[])
{
	const auto args = ParseGenerateArgs(argc, argv);
	std::ofstream output(args->outputFileName);
	GenerateNumbers(output, args->size);
}

void Sort(const int argc, char* argv[])
{
	const auto args = ParseSortArgs(argc, argv);
	std::ifstream input(args->inputFileName);
	std::ofstream output(args->outputFileName);

	std::vector<uint64_t> numbers;
	numbers.reserve(file_size(fs::path(args->inputFileName)) / 18);
	uint64_t num = 0;
	while (input >> num)
	{
		numbers.emplace_back(num);
	}

	for (int i = 1; i <= args->threadsNum; ++i)
	{
		auto copy = numbers;
		Sorter sorter(copy, i);
		MeasureTime(std::cout, std::to_string(i) + " thread sorting", &Sorter::Sort, std::ref(sorter));
		assert(std::ranges::is_sorted(copy));
	}
}

void ShowHelp()
{
	std::cout << R"(Usage:
sorter generate OUTPUT_FILE SIZE
    Generates OUTPUT_FILE containing SIZE pseudo-random integer numbers.

sorter sort INPUT_FILE OUTPUT_FILE MAX_THREADS
    Sort integer numbers in INPUT_FILE and writes result to OUTPUT_FILE.
    Numbers are sorted using up to MAX_THREADS
)";
}

int main(const int argc, char* argv[])
{
	if (argc < 2)
	{
		std::cout << "Invalid arguments count" << std::endl;
		return EXIT_FAILURE;
	}

	try
	{
		std::string mode = argv[1];
		if (mode == "generate")
		{
			Generate(argc, argv);
		}
		else if (mode == "sort")
		{
			Sort(argc, argv);
		}
		else if (mode == "help")
		{
			ShowHelp();
		}
		else
		{
			std::cout << "Unknown mode " + std::string(argv[1]) << std::endl;
		}
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}

	return EXIT_SUCCESS;
}
