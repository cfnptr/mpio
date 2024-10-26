// Copyright 2021-2024 Nikita Fediuchin. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "mpio/os.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if __linux__ || __APPLE__
#include <time.h>
#include <unistd.h>
	#if __x86_64__ || __i386__
	#include <cpuid.h>
	#endif
#elif _WIN32
#include <intrin.h>
#include <windows.h>
#else
#error Unknown operating system
#endif

#if __linux__
#include <sys/sysinfo.h>
#elif __APPLE__
#include <sys/sysctl.h>
#include <mach/mach_host.h>
#endif

#if __x86_64__ || _M_X64 || __i386__
#if __linux__ || __APPLE__
#define CPUID(id, cpuInfo) __cpuid(id, cpuInfo[0], cpuInfo[1], cpuInfo[2], cpuInfo[3])
#elif _WIN32
#define CPUID(id, cpuInfo) __cpuid((int*)cpuInfo, id)
#endif
#endif

double getCurrentClock()
{
#if __linux__ || __APPLE__
	struct timespec time;
	if (clock_gettime(CLOCK_MONOTONIC, &time) != 0) abort();
	return (double)time.tv_sec + (double)time.tv_nsec / 1000000000.0;
#elif _WIN32
	LARGE_INTEGER frequency;
	if (QueryPerformanceFrequency(&frequency) != TRUE)
		abort();
	LARGE_INTEGER counter;
	if (QueryPerformanceCounter(&counter) != TRUE)
		abort();
	return (double)counter.QuadPart / (double)frequency.QuadPart;
#endif
}

int getLogicalCpuCount()
{
	int cpuCount = -1;
#if __linux__
	cpuCount = (int)sysconf(_SC_NPROCESSORS_ONLN);

	if (cpuCount <= 0)
	{
		FILE* file = fopen("/proc/cpuinfo", "r");
		if (file)
		{
			char* buffer = malloc(256);
			if (!buffer)
			{
				fclose(file);
				return -1;
			}

			while (fgets(buffer, 256, file))
			{
				if (memcmp(buffer, "processor", 9) != 0)
					continue;

				char* pointer = memchr(buffer, ':', 256);
				if (!pointer)
					continue;

				int count = atoi(pointer + 1) + 1;
				if (count > cpuCount)
					cpuCount = count;
			}

			fclose(file);
		}
	}
#elif __APPLE__
	size_t size = sizeof(cpuCount);
	sysctlbyname("hw.logicalcpu", &cpuCount, &size, NULL, 0);

	if (cpuCount <= 0)
	{
		size = sizeof(cpuCount);
		sysctlbyname("machdep.cpu.thread_count", &cpuCount, &size, NULL, 0);
	}
	if (cpuCount <= 0)
	{
		size = sizeof(cpuCount);
		sysctlbyname("hw.activecpu", &cpuCount, &size, NULL, 0);
	}
#elif _WIN32
	SYSTEM_INFO systemInfo;
	GetSystemInfo(&systemInfo);
	cpuCount = (int)systemInfo.dwNumberOfProcessors;
#endif
	return cpuCount;
}

int getPhysicalCpuCount()
{
	int cpuCount = -1;
#if __linux__
	FILE* file = fopen("/proc/cpuinfo", "r");
	if (file)
	{
		char* buffer = malloc(256);
		if (!buffer)
		{
			fclose(file);
			return -1;
		}
		
		int processorCount = -1;
		while (fgets(buffer, 256, file))
		{
			if (memcmp(buffer, "cpu cores", 9) == 0)
			{
				char* pointer = memchr(buffer, ':', 256);
				if (!pointer)
					continue;

				cpuCount = atoi(pointer + 1);
				free(buffer);
				fclose(file);
				return cpuCount;
			}
			if (memcmp(buffer, "processor", 9) == 0)
			{
				char* pointer = memchr(buffer, ':', 256);
				if (!pointer)
					continue;

				int count = atoi(pointer + 1) + 1;
				if (count > processorCount)
					processorCount = count;
			}

			if (memcmp(buffer, "core id", 7) != 0)
				continue;

			char* pointer = memchr(buffer, ':', 256);
			if (!pointer)
				continue;

			int count = atoi(pointer + 1) + 1;
			if (count > cpuCount)
				cpuCount = count;
		}

		fclose(file);
	}

	if (cpuCount <= 0)
		cpuCount = processorCount;
#elif __APPLE__
	size_t size = sizeof(cpuCount);
	sysctlbyname("hw.physicalcpu", &cpuCount, &size, NULL, 0);

	if (cpuCount <= 0)
	{
		size = sizeof(cpuCount);
		sysctlbyname("machdep.cpu.core_count", &cpuCount, &size, NULL, 0);
	}
#elif _WIN32
	DWORD infoSize = 0;
	GetLogicalProcessorInformationEx(RelationProcessorCore, NULL, &infoSize);
	if (GetLastError() != ERROR_INSUFFICIENT_BUFFER)
		return -1;

	char* infoBuffer = malloc(infoSize);
	if (!infoBuffer)
		return -1;

	PSYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX info = 
		(PSYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX)infoBuffer;
	if (GetLogicalProcessorInformationEx(
		RelationProcessorCore, info, &infoSize) != TRUE)
	{
		free(infoBuffer);
		return -1;
	}

	cpuCount = 0;
	size_t offset = 0;

	do
	{
		const PSYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX currentInfo =
			(const PSYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX)(infoBuffer + offset);
		offset += currentInfo->Size;
		cpuCount++;
	}
	while (offset < infoSize);

	free(infoBuffer);
#endif
	return cpuCount;
}

int getPerformanceCpuCount()
{
	int cpuCount = -1;
#if __linux__
	// TODO: use /sys/devices/system/cpu/cpu0/cpu_capacity
#elif _WIN32
	DWORD infoSize = 0;
	GetLogicalProcessorInformationEx(RelationProcessorCore, NULL, &infoSize);
	if (GetLastError() != ERROR_INSUFFICIENT_BUFFER)
		return -1;

	char* infoBuffer = malloc(infoSize);
	if (!infoBuffer)
		return -1;

	PSYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX info =
		(PSYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX)infoBuffer;
	if (GetLogicalProcessorInformationEx(
		RelationProcessorCore, info, &infoSize) != TRUE)
	{
		free(infoBuffer);
		return -1;
	}

	cpuCount = 0;
	size_t offset = 0;

	do
	{
		const PSYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX currentInfo =
			(const PSYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX)(infoBuffer + offset);
		if (currentInfo->Relationship == RelationProcessorCore)
		{
			if (currentInfo->Processor.EfficiencyClass > 0)
				cpuCount++;
		}
		offset += currentInfo->Size;
	} while (offset < infoSize);

	free(infoBuffer);
#elif __APPLE__
	size_t size = sizeof(cpuCount);
	sysctlbyname("hw.perflevel0.physicalcpu", &cpuCount, &size, NULL, 0);
#endif
	if(cpuCount <= 0)
		cpuCount = getPhysicalCpuCount();
	return cpuCount;
}

int64_t getTotalRamSize()
{
#if __linux__
	struct sysinfo info;
	if (sysinfo(&info) != 0)
		return -1;
	return (int64_t)info.totalram * (int64_t)info.mem_unit;
#elif __APPLE__
	int mib [] = { CTL_HW, HW_MEMSIZE };
	int64_t value = 0;
	size_t length = sizeof(int64_t);
	if (sysctl(mib, 2, &value, &length, NULL, 0) != 0)
		return -1;
	return value;
#elif _WIN32
	MEMORYSTATUSEX statex;
	statex.dwLength = sizeof(statex);
	if (GlobalMemoryStatusEx(&statex) == FALSE)
		return -1;
	return statex.ullTotalPhys;
#endif
}

int64_t getFreeRamSize()
{
#if __linux__
	struct sysinfo info;
	if (sysinfo(&info) != 0)
		return -1;
	return (int64_t)info.freeram * (int64_t)info.mem_unit;
#elif __APPLE__
	mach_msg_type_number_t count = HOST_VM_INFO64_COUNT;
	vm_statistics64_data_t vmstat;
	if (host_statistics64(mach_host_self(), HOST_VM_INFO64,
		(host_info64_t)&vmstat, &count) != KERN_SUCCESS)
	{
		return -1;
	}
	return (vmstat.inactive_count + vmstat.free_count) * 
		(int64_t)getpagesize() / (1024 * 1024 * 1024);
#elif _WIN32
	MEMORYSTATUSEX statex;
	statex.dwLength = sizeof(statex);
	if (GlobalMemoryStatusEx(&statex) == FALSE)
		return -1;
	return statex.ullAvailPhys;
#endif
}

char* getCpuName()
{
	char* cpuName = calloc(65, sizeof(char));
	if (!cpuName)
		return NULL;

#if __x86_64__ || _M_X64 || __i386__
	unsigned int cpuInfo[4] = { 0, 0, 0, 0 };
	CPUID(0x80000000, cpuInfo);

	unsigned int nExIds = cpuInfo[0];
	for (unsigned int i = 0x80000000; i <= nExIds; ++i)
	{
		CPUID(i, cpuInfo);
		if (i == 0x80000002)
			memcpy(cpuName, cpuInfo, sizeof(cpuInfo));
		else if (i == 0x80000003)
			memcpy(cpuName + 16, cpuInfo, sizeof(cpuInfo));
		else if (i == 0x80000004)
			memcpy(cpuName + 32, cpuInfo, sizeof(cpuInfo));
	}
#else

	memcpy(cpuName, "Unknown\0", 8);

	#if __APPLE__
	size_t brandLength = 64;
	sysctlbyname("machdep.cpu.brand_string", cpuName, &brandLength, NULL, 0);
	#else
	FILE* file = fopen("/proc/cpuinfo", "r");
	if (file)
	{
		char* buffer = malloc(256);
		if (!buffer)
		{
			fclose(file);
			return cpuName;
		}

		while (fgets(buffer, 256, file))
		{
			if (memcmp(buffer, "model name", 10) != 0 &&
				memcmp(buffer, "Model", 5) != 0)
			{
				continue;
			}

			char* pointer = memchr(buffer, ':', 256);
			if (!pointer)
				continue;

			size_t index = (pointer - buffer) + 2;
			if (index >= 256)
				continue;

			for (size_t i = index; i < 256; i++)
			{
				char value = buffer[i];
				if (value != '\n' && value != '\0')
					continue;

				size_t count = i - index;
				if (i > 64)
				{
					free(buffer);
					fclose(file);
					return cpuName;
				}

				memcpy(cpuName, buffer + index, count);
				cpuName[count] = '\0';
				break;
			}
		}

		free(buffer);
		fclose(file);
	}
	#endif
#endif

	size_t nameLength = strlen(cpuName);
	for (int64_t i = nameLength - 1; i > 0; i--)
	{
		if (cpuName[i] != ' ')
		{
			nameLength = i + 1;
			break;
		}
	}
	cpuName[nameLength] = '\0';

	char* newCpuName = realloc(cpuName, nameLength + 1);
	if (!newCpuName)
	{
		free(cpuName);
		return NULL;
	}

	return newCpuName;
}