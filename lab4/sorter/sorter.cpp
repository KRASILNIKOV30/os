#include <vector>
#include <string>
#include <optional>
#include <iostream>

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

int main(int argc, char* argv[])
{

}
