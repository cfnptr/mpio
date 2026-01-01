# MPIO

A [library](https://github.com/cfnptr/mpio) providing generic interface for file system **input / output** operations across different platforms.

See the [documentation](https://cfnptr.github.io/mpio).

## Features

* Common directory and file functions
* App data and resources path getters
* CPU name (brand, model) getters
* Free and total RAM size getters
* Logical, physical, performance CPU count getters
* Current clock (time stamp) getter
* C and C++ implementations

## Supported operating systems

* Windows (10/11)
* Ubuntu (22.04/24.04)
* macOS (15/26)

This list includes only those systems on which functionality testing is conducted.
However, you can also compile it under any other Linux distribution or operating system.

## Build requirements

* C99 compiler
* C++17 compiler (optional)
* Objective-C compiler (macOS only)
* [Git 2.30+](https://git-scm.com/)
* [CMake 3.16+](https://cmake.org/)

Use building [instructions](BUILDING.md) to install all required tools and libraries.

### CMake options

| Name              | Description               | Default value |
|-------------------|---------------------------|---------------|
| MPIO_BUILD_SHARED | Build MPIO shared library | `ON`          |
| MPIO_BUILD_TESTS  | Build MPIO library tests  | `ON`          |

### CMake targets

| Name        | Description          | Windows | macOS    | Linux |
|-------------|----------------------|---------|----------|-------|
| mpio-static | Static MPIO library  | `.lib`  | `.a`     | `.a`  |
| mpio-shared | Dynamic MPIO library | `.dll`  | `.dylib` | `.so` |

## Cloning

```
git clone https://github.com/cfnptr/mpio
```

## Building ![CI](https://github.com/cfnptr/mpio/actions/workflows/cmake.yml/badge.svg)

* Windows: ```./scripts/build-release.bat```
* macOS / Ubuntu: ```./scripts/build-release.sh```
