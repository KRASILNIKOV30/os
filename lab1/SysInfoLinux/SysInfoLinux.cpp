#include "SysInfoLinux.hpp"

int main()
{
    try
    {
        PrintInfoLinux();
    }
    catch(std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }

    return 0;
}
