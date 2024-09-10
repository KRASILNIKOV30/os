#include "SysInfo.h"
#ifdef __linux__
#include "../SysInfoLinux/SysInfoLinux.hpp"
#elif _WIN32

#include <windows.h>
#include <VersionHelpers.h>
#include <sysinfoapi.h>

#else

#error "OS not supported!"

#endif

#ifdef __linux__

#elif _WIN32

#endif

std::string SysInfo::GetOSName() const
{
#ifdef __linux__
    return GetLinuxOSName();
#elif _WIN32

#endif
}

std::string SysInfo::GetOSVersion() const
{
#ifdef __linux__
    return GetLinuxOSVersion();
#elif _WIN32

#endif
}

uint64_t SysInfo::GetFreeMemory() const
{
#ifdef __linux__
    return GetLinuxFreeMemory();
#elif _WIN32

#endif
}

uint64_t SysInfo::GetTotalMemory() const
{
#ifdef __linux__
    return GetLinuxTotalMemory();
#elif _WIN32

#endif
}

unsigned SysInfo::GetProcessorCount() const
{
#ifdef __linux__
    return GetLinuxProcCount();
#elif _WIN32

#endif
}
