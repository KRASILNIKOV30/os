#include <iostream>

#include "SysInfo.h"

int main()
{
    try
    {
        SysInfo sysInfo;
        std::cout << "OS: " << sysInfo.GetOSName() << " " << sysInfo.GetOSVersion() << std::endl;
        std::cout << "RAM: " << sysInfo.GetFreeMemory() << "B / ";
        std::cout << sysInfo.GetTotalMemory() << "B" << std::endl;
        std::cout << "Processors: " << sysInfo.GetProcessorCount() << std::endl;
    }
    catch(std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }
}
