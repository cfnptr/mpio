// Copyright 2021-2022 Nikita Fediuchin. All rights reserved.
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

#if __linux__ || __APPLE__
#include <stdbool.h>
#include <sys/stat.h>

#define createDirectory(x) mkdir(x, 0777) == 0

inline static bool isDirectoryExists(const char* path)
{
	struct stat sb;
	return stat(path, &sb) == 0 && S_ISDIR(sb.st_mode);
}
#elif _WIN32
#include <stdbool.h>
#include <windows.h>

#define createDirectory(x) CreateDirectoryA(x, NULL) == TRUE

inline static bool isDirectoryExists(const char* path)
{
	DWORD attribs = GetFileAttributes(path);

	return (attribs != INVALID_FILE_ATTRIBUTES &&
		(attribs & FILE_ATTRIBUTE_DIRECTORY));
}
#else
#error Unknown operating system
#endif
