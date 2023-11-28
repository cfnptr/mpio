# MPIO ![CI](https://github.com/cfnptr/mpio/actions/workflows/cmake.yml/badge.svg)

A library providing generic interface for file system **input / output** operations across different platforms.

## Features

* Common director and file functions
* App data and resources path getters
* CPU name, RAM size, CPU count getters
* Current clock (time) getter

## Supported operating systems

* Ubuntu
* MacOS
* Windows

## Build requirements

* C99 compiler
* C++17 compiler (optional)
* Objective-C compiler (macOS only)
* [Git 2.30+](https://git-scm.com/)
* [CMake 3.16+](https://cmake.org/)

### CMake options

| Name              | Description               | Default value |
|-------------------|---------------------------|---------------|
| MPIO_BUILD_SHARED | Build MPIO shared library | `ON`          |

## Cloning

```
git clone https://github.com/cfnptr/mpio
```