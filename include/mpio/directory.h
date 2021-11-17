#pragma once

#if __linux__ || __APPLE__
#include <sys/stat.h>
#define createDirectory(x) mkdir(x, 0777) == 0
#elif _WIN32
#include <windows.h>
#define createDirectory(x) CreateDirectoryA(x, NULL) == TRUE
#else
#error Unknown operating system
#endif
