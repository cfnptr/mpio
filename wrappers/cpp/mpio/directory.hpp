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

extern "C"
{
#include "mpio/directory.h"
}

#include <filesystem>

namespace mpio
{

using namespace std;

class Directory
{
public:
	/*
	 * Create a new directory.
	 * Returns true on success.
	 *
	 * path - directory path string.
	 */
	static bool create(const filesystem::path& path)
	{
		auto string = path.generic_string();
		return createDirectory(string.c_str());
	}
	/*
	 * Returns true if directory exists.
	 * path - directory path string.
	 */
	static bool isExists(const filesystem::path& path)
	{
		auto string = path.generic_string();
		return isDirectoryExists(string.c_str());
	}

	/*
	 * Returns application data directory, or throws on failure.
	 * isShared - is directory shared between users.
	 */
	static filesystem::path getDataPath(bool isShared = false)
	{
		auto dataPath = getDataDirectory(isShared);
		if (!dataPath) throw runtime_error("Failed to get data directory.");
		auto path = filesystem::path(dataPath);
		free(dataPath);
		return path;
	}
	/*
	 * Returns application data directory + name, or throws on failure.
	 *
	 * appName - application name string.
	 * isShared - is directory shared between users.
	 */
	static filesystem::path getAppDataPath(const string& appName, bool isShared = false)
	{
		auto dataPath = getAppDataDirectory(appName.c_str(), isShared);
		if (!dataPath) throw runtime_error("Failed to get app data directory.");
		auto path = filesystem::path(dataPath);
		free(dataPath);
		return path;
	}
	/*
	 * Returns bundle resources directory, or throws on failure.
	 */
	static filesystem::path getResourcesPath()
	{
		auto dataPath = getResourcesDirectory();
		if (!dataPath) throw runtime_error("Failed to get resources directory.");
		auto path = filesystem::path(dataPath);
		free(dataPath);
		return path;
	}
};

} // mpio