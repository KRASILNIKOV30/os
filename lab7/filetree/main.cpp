#include <iostream>
#include <filesystem>
#include <string>
#include <vector>

namespace fs = std::filesystem;

void PrintTree(const fs::path& dirPath, const std::string& prevIndent = "");

void PrintFiles(std::vector<fs::directory_entry> const& files, std::string const& indent)
{

	for (const auto& entry : files)
	{
		if (is_regular_file(entry.status()))
		{
			std::cout << indent << "[F] " << entry.path().filename().string() << std::endl;
		}
	}

	if (!files.empty())
	{
		std::cout << indent << std::endl;
	}
}

void PrintDirs(std::vector<fs::directory_entry> const& dirs, std::string const& prevIndent)
{
	for (size_t i = 0; i < dirs.size(); ++i)
	{
		const bool isLast = i == dirs.size() - 1;
		const auto& dir = dirs[i];
		std::cout << prevIndent << (isLast ? "\\---" : "+---") << "[D] " << dir.path().filename().string() << std::endl;
		PrintTree(dir.path(), prevIndent + (isLast ? "    " : "|   "));
	}
}

void PrintTree(const fs::path& dirPath, const std::string& prevIndent)
{
	std::vector<fs::directory_entry> files;
	std::vector<fs::directory_entry> dirs;

	try
	{
		for (const auto& entry : fs::directory_iterator(dirPath))
		{
			if (entry.is_regular_file())
			{
				files.push_back(entry);
			}
			if (entry.is_directory())
			{
				dirs.push_back(entry);
			}
		}
	}
	catch (const fs::filesystem_error& e)
	{
		std::cerr << "Error reading directory " << dirPath << ": " << e.what() << std::endl;
		return;
	}

	const auto indent = prevIndent + (!dirs.empty() ? "|   " : "    ");
	PrintFiles(files, indent);
	PrintDirs(dirs, prevIndent);
}

int main(const int argc, char* argv[])
{
	fs::path dirPath = ".";
	if (argc > 1)
	{
		dirPath = argv[1];
	}

	try
	{
		if (exists(dirPath) && is_directory(dirPath))
		{
			std::cout << dirPath.filename().string() << std::endl;
			PrintTree(dirPath);
		} else
		{
			std::cerr << "There is no directory in path\n";
			return 1;
		}
	}
	catch (const fs::filesystem_error& e)
	{
		std::cerr << e.what() << std::endl;
		return 1;
	}

	return 0;
}
