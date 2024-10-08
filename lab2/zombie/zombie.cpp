#include <iostream>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
	auto pid = fork();

	if (pid < 0)
	{
		std::cout << "fork failed" << std::endl;
		return EXIT_FAILURE;
	}

	if(pid == 0)
	{
		return EXIT_SUCCESS;
	}

	do
	{
		std::cout << "enter child pid: " << std::endl;
		std::cin >> pid;
	} while (pid <= 0 || waitpid(pid, nullptr, 0) == -1);
}
