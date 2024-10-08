#include "SysInfo.h"
#include <string>

#ifdef __linux__
#include "../sysInfoLinux/SysInfoLinux.hpp"
#elif _WIN32
#include "../SysInfoWin/SysInfoWin.hpp"
#else
#error "OS not supported!"
#endif

namespace
{
#ifdef __linux__
    std::string GetOSNameImpl()
    {
        return GetLinuxOSName();
    }

    std::string GetOSVersionImpl()
    {
        return GetLinuxOSVersion();
    }

    uint64_t GetFreeMemoryImpl()
    {
        return GetLinuxFreeMemory();
    }

    uint64_t GetTotalMemoryImpl()
    {
        return GetLinuxTotalMemory();
    }

    unsigned GetProcessorCountImpl()
    {
        return GetLinuxProcCount();
    }
#elif _WIN32
    std::string GetOSNameImpl()
    {
        return GetWinOSName();
    }

    std::string GetOSVersionImpl()
    {
        return GetWinVersion();
    }

    uint64_t GetFreeMemoryImpl()
    {
        return GetWinFreeMemory();
    }

    uint64_t GetTotalMemoryImpl()
    {
        return GetWinTotalMemory();
    }

    unsigned GetProcessorCountImpl()
    {
        return GetWinProcCount();
    }
#else
#error "OS not supported!"
#endif
}

std::string SysInfo::GetOSName() const
{
    return GetOSNameImpl();
}

std::string SysInfo::GetOSVersion() const
{
    return GetOSVersionImpl();
}

uint64_t SysInfo::GetFreeMemory() const
{
    return GetFreeMemoryImpl();
}

uint64_t SysInfo::GetTotalMemory() const
{
    return GetTotalMemoryImpl();
}

unsigned SysInfo::GetProcessorCount() const
{
    return GetProcessorCountImpl();
}
