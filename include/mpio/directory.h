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

/*
 * Create a new directory. (MT-Safe)
 * Returns true on success.
 *
 * path - directory path string.
 */
bool createDirectory(const char* path);
/*
 * Returns true if directory exists. (MT-Safe)
 * path - directory path string.
 */
bool isDirectoryExists(const char* path);

/*
 * Returns application data directory, or NULL on failure. (MT-Safe)
 * Allocated string is not automatically destroyed!
 * 
 * isShared - is directory shared between users.
 */
char* getDataDirectory(bool isShared);
/*
 * Returns application data directory + name, or NULL on failure. (MT-Safe)
 * Allocated string is not automatically destroyed!
 *
 * appName - application name string.
 * isShared - is directory shared between users.
 */
char* getAppDataDirectory(const char* appName, bool isShared);
/*
 * Returns bundle resources directory, or NULL on failure. (MT-Safe)
 * Allocated string is not automatically destroyed!
 */
char* getResourcesDirectory();