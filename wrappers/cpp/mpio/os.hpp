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

/***********************************************************************************************************************
 * @file
 * @brief Common operating system functions.
 * @details See the @ref os.h
 **********************************************************************************************************************/

#pragma once
#include <string>

extern "C"
{
#include "mpio/os.h"
}

namespace mpio
{

using namespace std;

/**
 * @brief Common operating system functions.
 * @details See the @ref os.h
 */
class OS
{
public:
	/**
	 * @brief Returns high resolution time stamp in seconds. (MT-Safe)
	 * @details See the @ref getCurrentClock().
	 */
	static double getCurrentClock() noexcept
	{
		return ::getCurrentClock();
	}

	/**
	 * @brief Returns system logical CPU count. (MT-Safe)
	 * @details See the @ref getCpuCount().
	 */
	static int getCpuCount() noexcept
	{
		return ::getCpuCount();
	}

	/**
	 * @brief Returns system total physical RAM size. (MT-Safe)
	 * @details See the @ref getTotalRamSize().
	 * @return The total RAM size in bytes on success, otherwise -1.
	 */
	static int64_t getTotalRamSize() noexcept
	{
		return ::getTotalRamSize();
	}

	/**
	 * @brief Returns system free physical RAM size. (MT-Safe)
	 * @details See the @ref getFreeRamSize().
	 * @return The free RAM size in bytes on success, otherwise -1.
	 */
	static int64_t getFreeRamSize() noexcept
	{
		return ::getFreeRamSize();
	}

	/**
	 * @brief Returns system CPU name string. (MT-Safe)
	 * @details See the @ref getCpuName().
	 * @throw runtime_error if failed to get CPU name.
	 */
	static string getCpuName()
	{
		auto cpuName = ::getCpuName();
		if (!cpuName) throw runtime_error("Failed to get CPU name.");
		auto cpuString = string(cpuName);
		free(cpuName);
		return cpuString;
	}
};

}; // mpio