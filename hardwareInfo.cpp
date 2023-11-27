

#include <iostream>

#if (defined(_WIN64) || defined(__WIN32))
#include <windows.h>
#include <malloc.h>
#include <cstdio>
#include <tchar.h>

typedef BOOL (WINAPI *LPFN_GLPI)(
        PSYSTEM_LOGICAL_PROCESSOR_INFORMATION,
        PDWORD);


// Helper function to count set bits in the processor mask.
DWORD CountSetBits(ULONG_PTR bitMask)
{
    DWORD LSHIFT = sizeof(ULONG_PTR)*8 - 1;
    DWORD bitSetCount = 0;
    ULONG_PTR bitTest = (ULONG_PTR)1 << LSHIFT;
    DWORD i;

    for (i = 0; i <= LSHIFT; ++i)
    {
        bitSetCount += ((bitMask & bitTest)?1:0);
        bitTest/=2;
    }

    return bitSetCount;
}

#endif

#if defined(__APPLE__)
#include <sys/types.h>
#include <sys/sysctl.h>
#endif

#if defined(__linux__)
#include <fstream>
#include <string>
#include <set>
#endif

bool getLogicalProcessorsCount(unsigned int &logicalPcount)
{
#if (defined(_WIN64) || defined(__WIN32))
    LPFN_GLPI glpi;
    BOOL done = FALSE;
    PSYSTEM_LOGICAL_PROCESSOR_INFORMATION buffer = NULL;
    PSYSTEM_LOGICAL_PROCESSOR_INFORMATION ptr = NULL;
    DWORD returnLength = 0;
    DWORD logicalProcessorCount = 0;
    DWORD numaNodeCount = 0;
    DWORD processorCoreCount = 0;
    DWORD processorL1CacheCount = 0;
    DWORD processorL2CacheCount = 0;
    DWORD processorL3CacheCount = 0;
    DWORD processorPackageCount = 0;
    DWORD byteOffset = 0;
    PCACHE_DESCRIPTOR Cache;

    glpi = (LPFN_GLPI) GetProcAddress(
            GetModuleHandle(TEXT("kernel32")),
            "GetLogicalProcessorInformation");
    if (NULL == glpi)
    {
        _tprintf(TEXT("\nGetLogicalProcessorInformation is not supported.\n"));
        return false;
    }

    while (!done)
    {
        DWORD rc = glpi(buffer, &returnLength);

        if (FALSE == rc)
        {
            if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
            {
                if (buffer)
                    free(buffer);

                buffer = (PSYSTEM_LOGICAL_PROCESSOR_INFORMATION)malloc(
                        returnLength);

                if (NULL == buffer)
                {
                    _tprintf(TEXT("\nError: Allocation failure\n"));
                    return false;
                }
            }
            else
            {
                _tprintf(TEXT("\nError %d\n"), GetLastError());
                return false;
            }
        }
        else
        {
            done = TRUE;
        }
    }

    ptr = buffer;

    while (byteOffset + sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION) <= returnLength)
    {
        switch (ptr->Relationship)
        {
            case RelationNumaNode:
                // Non-NUMA systems report a single record of this type.
                numaNodeCount++;
                break;

            case RelationProcessorCore:
                processorCoreCount++;

                // A hyperthreaded core supplies more than one logical processor.
                logicalProcessorCount += CountSetBits(ptr->ProcessorMask);
                break;

            case RelationCache:
                // Cache data is in ptr->Cache, one CACHE_DESCRIPTOR structure for each cache.
                Cache = &ptr->Cache;
                if (Cache->Level == 1)
                {
                    processorL1CacheCount++;
                }
                else if (Cache->Level == 2)
                {
                    processorL2CacheCount++;
                }
                else if (Cache->Level == 3)
                {
                    processorL3CacheCount++;
                }
                break;

            case RelationProcessorPackage:
                // Logical processors share a physical package.
                processorPackageCount++;
                break;

            default:
                _tprintf(TEXT("\nError: Unsupported LOGICAL_PROCESSOR_RELATIONSHIP value.\n"));
                break;
        }
        byteOffset += sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION);
        ptr++;
    }

    logicalPcount = logicalProcessorCount;

    return true;
#endif

#if defined(__APPLE__)

    int numCPU;
    size_t numCPULen = sizeof(numCPU);

    sysctlbyname("hw.logicalcpu", &numCPU, &numCPULen, NULL, 0);

    logicalPcount = numCPU;

    return true;

#endif

#if defined(__linux__)

    std::ifstream cpuinfo("/proc/cpuinfo");
    std::string line;
    std::set<int> cores;
    while (std::getline(cpuinfo, line)) {
        if (line.substr(0, 9) == "processor") {
            cores.insert(std::stoi(line.substr(11)));
        }
    }

    logicalPcount = cores.size();

    if(logicalPcount > 0)
        return true;
    else
        return false;

#endif


}