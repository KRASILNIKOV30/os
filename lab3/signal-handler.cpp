#include <iostream>
#include <csignal>

void handler(const int signum)
{
    std::cout << "signal " << signum << " received" << std::endl;
}

int main()
{
    if (std::signal(SIGTERM, handler) == SIG_ERR)
    {
        std::cerr << "signal error" << std::endl;
    }
    char ch;
    std::cin >> ch;

    return EXIT_SUCCESS;
}


