#include <iostream>
#include <sys/utsname.h>
#include <sys/sysinfo.h>

namespace
{
    constexpr int BYTES_IN_KILOBYTE = 1024;
    constexpr int KILOBYTES_IN_MEGABYTE = 1024;

    uint64_t BToMb(const uint64_t bytes)
    {
        return bytes / BYTES_IN_KILOBYTE / KILOBYTES_IN_MEGABYTE;
    }
}


void DisplayInfo
(
    std::string const& osInfo,
    std::string const& ramInfo,
    std::string const& procInfo
)
{
    std::cout << "OS: " << osInfo << std::endl;
    std::cout << "RAM: " << ramInfo << std::endl;
    std::cout << "Processors: " << procInfo << std::endl;
}

std::string GetLinuxOSName()
{
    utsname unameData;
    if (uname(&unameData) < 0)
    {
        return "uname() failed";
    }
    return std::string(unameData.sysname);
}

std::string GetLinuxOSVersion()
{
    utsname unameData;
    if (uname(&unameData) < 0)
    {
        return "uname() failed";
    }
    return std::string(unameData.version);
}

std::string GetOSInfo()
{
    return GetLinuxOSName() + " " + GetLinuxOSVersion();
}

uint64_t GetLinuxTotalMemory()
{
    struct sysinfo sysInfo;
    // Добавить проверки возвр значения (Исправлено)
    if (sysinfo(&sysInfo) != 0)
    {
        throw std::runtime_error("sysinfo() failed");
    }
    return sysInfo.totalram;
}

uint64_t GetLinuxFreeMemory()
{
    struct sysinfo sysInfo;
    // Добавить проверки возвр значения (Исправлено)
    if (sysinfo(&sysInfo) != 0)
    {
        throw std::runtime_error("sysinfo() failed");
    }
    return sysInfo.freeram;
}

std::string GetLinuxRAMInfo()
{
    const auto total = BToMb(GetLinuxTotalMemory());
    const auto free = BToMb(GetLinuxFreeMemory());

    return std::to_string(free) + "MB / " + std::to_string(total) + "MB";
}

unsigned GetLinuxProcCount()
{
    return get_nprocs_conf();
}

void PrintInfoLinux()
{
    DisplayInfo(GetOSInfo(), GetLinuxRAMInfo(), std::to_string(GetLinuxProcCount()));
}