#pragma once
#include "windows.h"
#include "psapi.h"

class ComputerResourcesHelper
{
public:
	static SIZE_T GetRamUsage()
	{
		PROCESS_MEMORY_COUNTERS pmc;
		GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc));
		SIZE_T virtualMemUsedByMe = pmc.WorkingSetSize;
		return virtualMemUsedByMe;
	}
};

