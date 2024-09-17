#include "SysInfoWin.hpp"

int main()
{
    try
    {
        PrintInfoWin();
    }
    catch(std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }

    return 0;
}
