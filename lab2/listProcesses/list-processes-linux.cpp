#include <filesystem>
#include <fstream>
#include <iostream>
#include <bits/ranges_algo.h>

namespace fs = std::filesystem;

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
		if(!std::ranges::all_of(filename, [](const char c){ return std::isdigit(c); }))
		{
			continue;
		}
		std::ifstream file(fileEntry.path().string() + "/status");
		std::cout << filename << ": " << std::endl;
		std::string line;
		while (std::getline(file, line))
		{
			std::string field = line.substr(0, line.find(' '));
			if (field == "Name:")
			{
				std::cout << line << std::endl;
			}
			if (field == "VmSize:")
			{
				std::cout << line << std::endl;
			}
		}
		std::cout << std::endl;
	}
}
