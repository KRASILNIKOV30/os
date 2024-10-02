#include <windows.h>
#include <psapi.h>
#include <iostream>

void PrintProcessInfo(DWORD pid)
{
    TCHAR szProcessName[MAX_PATH] = "<unknown>";
    PROCESS_MEMORY_COUNTERS pmc;

    HANDLE hProcess = OpenProcess(
        PROCESS_QUERY_INFORMATION |PROCESS_VM_READ,
        FALSE,
        pid
    );

    if (hProcess != NULL)
    {
        HMODULE hMod;
        DWORD cbNeeded;

        if (EnumProcessModules(hProcess, &hMod, sizeof(hMod), &cbNeeded) )
        {
            auto size = sizeof(szProcessName) / sizeof(TCHAR);
            GetModuleBaseName(hProcess, hMod, szProcessName, size);
            if (GetProcessMemoryInfo( hProcess, &pmc, sizeof(pmc)) == 0)
            {
                throw std::runtime_error("GetProcessMemoryInfo failed");
            }
        }
    }

    std::cout << pid << ": " << szProcessName << " " << pmc.WorkingSetSize / 1024 / 1024 << "MB" << std::endl;

    CloseHandle( hProcess );
}

int main()
{
    DWORD pids[1024];
    DWORD bytesReturned;
    DWORD procCount;

    if (!EnumProcesses(pids, sizeof(pids), &bytesReturned ) )
    {
        return 1;
    }

    procCount = bytesReturned / sizeof(DWORD);
    unsigned int i;
    for ( i = 0; i < procCount; i++ )
    {
        if(pids[i] != 0 )
        {
            try
            {
                PrintProcessInfo(pids[i]);
            }
            catch (std::exception const& e)
            {
               std::cout << e.what() << std::endl;
            }
        }
    }

    return 0;
}