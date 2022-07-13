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

#include "mpio/directory.h"
#include <sys/param.h>
#include <CoreFoundation/CFBundle.h>
#include <Foundation/NSFileManager.h>

static char macosPath[MAXPATHLEN + 1];

const char* getDataDirectory(bool isShared)
{
	NSArray<NSString*>* array = NSSearchPathForDirectoriesInDomains(
		NSApplicationSupportDirectory,
		isShared ? NSLocalDomainMask : NSUserDomainMask,
		YES);

	if (!array)
		return NULL;

	NSString* string = [array lastObject];

	if (!string)
		return NULL;

	return [string UTF8String];
}
const char* getAppDataDirectory(
	const char* appName,
	bool isShared)
{
	const char* dataDirectory = getDataDirectory(isShared);

	if (!dataDirectory)
		return false;

	size_t dataPathLength = strlen(dataDirectory);
	size_t appNameLength = strlen(appName);
	size_t pathLength = dataPathLength + appNameLength + 1;

	if (pathLength > MAXPATHLEN)
		return false;

	memcpy(macosPath, dataDirectory,
		dataPathLength * sizeof(char));
	macosPath[dataPathLength] = '/';
	memcpy(macosPath + dataPathLength + 1, appName,
		appNameLength * sizeof(char));
	macosPath[dataPathLength + appNameLength + 1] = '\0';
	return macosPath;
}
const char* getResourcesDirectory()
{
	CFBundleRef bundle = CFBundleGetMainBundle();

	if (!bundle)
		return NULL;

	CFURLRef resourcesURL = CFBundleCopyResourcesDirectoryURL(bundle);

	if (!CFURLGetFileSystemRepresentation(resourcesURL,
		true,  (UInt8*)macosPath, MAXPATHLEN))
	{
		CFRelease(resourcesURL);
		return NULL;
	}

	CFRelease(resourcesURL);
	return macosPath;
}
