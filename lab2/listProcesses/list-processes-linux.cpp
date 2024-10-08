#include <filesystem>
#include <fstream>
#include <iostream>
#include <bits/ranges_algo.h>

namespace fs = std::filesystem;

void PrintProcessInfo(fs::directory_entry const& entry)
{
	auto filename = entry.path().filename().string();
	std::ifstream file(entry.path() / "status");
	std::cout << filename << ": " << std::endl;

	std::string line;
	while (std::getline(file, line))
	{
		std::string field = line.substr(0, line.find('\t'));
		if (field == "Name:")
		{
			std::cout << line << std::endl;
		}
		if (field == "VmSize:")
		{
			std::cout << line << std::endl;
		}
	}
}

int main()
{
	std::string dirPath("/proc");

	for (const auto& fileEntry : fs::directory_iterator(dirPath))
	{
		if (!is_directory(fileEntry))
		{
			continue;
		}
		auto filename = fileEntry.path().filename().string();
		if(!std::ranges::all_of(filename, isdigit))
		{
			continue;
		}

		PrintProcessInfo(fileEntry);
		std::cout << std::endl;
	}
}
