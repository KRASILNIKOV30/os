#include <iostream>
#include <sys/utsname.h>
#include <sys/sysinfo.h>

constexpr int BYTES_IN_KILOBYTE = 1024;
constexpr int KILOBYTES_IN_MEGABYTE = 1024;

unsigned long BToMb(const unsigned long bytes)
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

std::string GetOSInfo()
{
    utsname unameData;
    uname(&unameData);
    return std::string(unameData.sysname) + " " + unameData.version;
}

std::string GetRAMInfo()
{
    struct sysinfo sysInfo;
    sysinfo(&sysInfo);
    const auto total = BToMb(sysInfo.totalram);
    const auto free = BToMb(sysInfo.freeram);
    return std::to_string(free) + "MB / " + std::to_string(total) + "MB";
}

std::string GetProcInfo()
{
    return std::to_string(get_nprocs_conf());
}

void PrintInfoLinux()
{
    DisplayInfo(GetOSInfo(), GetRAMInfo(), GetProcInfo());
}

int main()
{
    PrintInfoLinux();

    return 0;
}