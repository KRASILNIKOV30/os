#include "CurrentTime.h"
#include <syncstream>
#include <iostream>
#include <thread>

void Tic(std::osyncstream& output, std::stop_token const& stoken)
{
	while (!stoken.stop_requested())
	{
		output << CurrentTimeToString() << " Tic" << std::endl;
		output.emit();
		std::this_thread::sleep_for(std::chrono::seconds(2));
	}
	output << "Tic thread completed" << std::endl;
}

void Tac(std::osyncstream& output, std::stop_token const& stoken)
{
	while (!stoken.stop_requested())
	{
		output << CurrentTimeToString() << " Tac" << std::endl;
		output.emit();
		std::this_thread::sleep_for(std::chrono::seconds(3));
	}
	output << "Tac thread completed" << std::endl;
}

int main()
{
	std::osyncstream syncOut(std::cout);
	const std::stop_source stopSource;
	std::jthread ticThread(Tic, std::ref(syncOut), stopSource.get_token());
	std::jthread tacThread(Tac, std::ref(syncOut), stopSource.get_token());

	std::string buf;
	std::cin >> buf;
	if (!stopSource.request_stop())
	{
		std::cout << "requested stop failed" << std::endl;
	}

	return 0;
}
