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
#include <stdbool.h>

#if __linux__ || __APPLE__
/*
 * Create a new directory.
 * Returns true on success.
 *
 * path - directory path string.
 */
bool createDirectory(const char* path);
/*
 * Returns true if directory exists.
 * path - directory path string.
 */
bool isDirectoryExists(const char* path);
#elif _WIN32
/*
 * Create a new directory.
 * Returns true on success.
 *
 * path - directory path string.
 */
bool createDirectory(const char* path);
/*
 * Returns true if directory exists.
 * path - directory path string.
 */
bool isDirectoryExists(const char* path);
#else
#error Unknown operating system
#endif

#if __APPLE__
/*
 * Returns macOS program data directory, or NULL on failure.
 * isShared - is directory shared between users.
 */
const char* getDataDirectory(bool isShared);
/*
 * Returns macOS bundle resources directory, or NULL on failure.
 */
const char* getResourcesDirectory();
#endif
