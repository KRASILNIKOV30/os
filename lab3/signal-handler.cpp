#include <chrono>
#include <iostream>
#include <csignal>
#include <fstream>
#include <thread>
#include <filesystem>
#include <array>

namespace fs = std::filesystem;

constexpr int COUNT = 100;
constexpr auto TEMP_FILE = "temp.txt";
std::atomic_flag exitRequested;

void FinishProgram()
{
	if (fs::remove(TEMP_FILE))
	{
		std::cout << "Done" << std::endl;
	}
	else
	{
		std::cout << "cannot remove temp file" << std::endl;
	}
}

void SignalHandler(const int signum)
{
	if (signum == SIGINT || signum == SIGTERM)
	{
		std::cout << "Process " << getpid() << " has received signal #" << signum << std::endl;
		throw std::runtime_error("Runtime error");

		exitRequested.test_and_set();
	}
}

void DoCount()
{
	for (int i = 0; i <= COUNT; i++)
	{
		std::this_thread::sleep_for(std::chrono::seconds(1));
		std::cout << i << std::endl;
		if (exitRequested.test())
		{
			return;
		}
	}
}

int main()
{
	try
	{
		std::array signals = { SIGTERM, SIGINT };
		for (const auto& signal : signals)
		{
			if (std::signal(signal, SignalHandler) == SIG_ERR)
			{
				std::cerr << "std::signal failed" << std::endl;
			}
		}
		auto file = std::ofstream(TEMP_FILE);

		DoCount();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << " was caught" << std::endl;
	}

	FinishProgram();
	return EXIT_SUCCESS;
}