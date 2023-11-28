// Copyright 2021-2023 Nikita Fediuchin. All rights reserved.
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

#pragma once
#include <string_view>

extern "C"
{
#include "mpio/os.h"
}

namespace mpio
{

using namespace std;

class OS
{
public:
	/*
	 * Returns current clock value (s).
	 */
	static double getCurrentClock() noexcept
	{
		return ::getCurrentClock();
	}

	/*
	 * Returns running system logical CPU count.
	 */
	static int getCpuCount() noexcept
	{
		return ::getCpuCount();
	}

	/*
	 * Returns running system total RAM size.
	 */
	static int64_t getRamSize() noexcept
	{
		return ::getRamSize();
	}

	/*
	 * Returns running system CPU name string.
	 */
	static string_view getCpuName() noexcept
	{
		return ::getCpuName();
	}
};

}; // mpio