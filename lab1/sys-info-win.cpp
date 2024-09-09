#include <iostream>
#include <windows.h>
#include <VersionHelpers.h>
#include <format>
#include <sysinfoapi.h>

constexpr int BYTES_IN_KILOBYTE = 1024;
constexpr int KILOBYTES_IN_MEGABYTE = 1024;

DWORDLONG BToMb(DWORDLONG bytes)
{
    return bytes / BYTES_IN_KILOBYTE / KILOBYTES_IN_MEGABYTE;
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

std::string GetWinVersionMessage(std::string const& v)
{
    return std::format("Windows {} or greater", v);
}

std::string GetWinVersion()
{
    if (IsWindowsVersionOrGreater(11, 0, 0))
    {
        return GetWinVersionMessage("11");
    }
    if (IsWindows10OrGreater())
    {
        return GetWinVersionMessage("10");
    }
    if (IsWindows8Point1OrGreater())
    {
        return GetWinVersionMessage("8.1");
    }
    if (IsWindows8OrGreater())
    {
        return GetWinVersionMessage("8");
    }
    if (IsWindows7SP1OrGreater())
    {
        return GetWinVersionMessage("7 with Service Pack 1");
    }
    if (IsWindows7OrGreater())
    {
        return GetWinVersionMessage("7");
    }
    if (IsWindowsVistaSP2OrGreater())
    {
        return GetWinVersionMessage("Vista with Service Pack 2");
    }
    if (IsWindowsVistaSP1OrGreater())
    {
        return GetWinVersionMessage("Vista with Service Pack 1");
    }
    if (IsWindowsVistaOrGreater())
    {
        return GetWinVersionMessage("Vista");
    }
    if (IsWindowsXPSP3OrGreater())
    {
        return GetWinVersionMessage("XP with Service Pack 3");
    }
    if (IsWindowsXPSP2OrGreater())
    {
        return GetWinVersionMessage("XP with Service Pack 2");
    }
    if (IsWindowsXPSP1OrGreater())
    {
        return GetWinVersionMessage("XP with Service Pack 1");
    }
    if (IsWindowsXPOrGreater())
    {
        return GetWinVersionMessage("XP");
    }

    return "Windows version is less than Windows XP";
}

std::string GetProcInfo()
{
    SYSTEM_INFO info;
    GetSystemInfo(&info);
    return std::to_string(info.dwNumberOfProcessors);
}

std::string GetRamInfo()
{
    MEMORYSTATUSEX info;
    info.dwLength = sizeof(MEMORYSTATUSEX);
    GlobalMemoryStatusEx(&info);
    auto total = info.ullTotalPhys;
    auto inUse = total / 100 * info.dwMemoryLoad;
    return std::format("{}MB / {}MB", BToMb(inUse), BToMb(total));
}

int main()
{
    DisplayInfo(GetWinVersion(), GetRamInfo(), GetProcInfo());
}
