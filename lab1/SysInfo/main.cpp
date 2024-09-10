#include <iostream>

#include "SysInfo.h"

namespace
{
    constexpr int BYTES_IN_KILOBYTE = 1024;
    constexpr int KILOBYTES_IN_MEGABYTE = 1024;

    uint64_t BToMb(const uint64_t bytes)
    {
        return bytes / BYTES_IN_KILOBYTE / KILOBYTES_IN_MEGABYTE;
    }
}

int main()
{
    try
    {
        SysInfo sysInfo;
        std::cout << "OS: " << sysInfo.GetOSName() << " " << sysInfo.GetOSVersion() << std::endl;
        std::cout << "RAM: " << BToMb(sysInfo.GetFreeMemory()) << "MB / ";
        std::cout << BToMb(sysInfo.GetTotalMemory()) << "MB" << std::endl;
        std::cout << "Processors: " << sysInfo.GetProcessorCount() << std::endl;
    }
    catch(std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }
}
