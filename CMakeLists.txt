# Copyright 2021-2022 Nikita Fediuchin. All rights reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

cmake_minimum_required(VERSION 3.20)

if (${CMAKE_SOURCE_DIR} MATCHES ${CMAKE_BINARY_DIR})
	message(FATAL_ERROR "Prevented in-tree build")
endif ()

if (TARGET mpio-static)
	return()
endif ()

set(MPIO_LANGUAGES C)

if (CMAKE_SYSTEM_NAME MATCHES "Darwin")
	list(APPEND MPIO_LANGUAGES OBJC)
endif ()

project(mpio
	VERSION 0.0.1
	DESCRIPTION "Multi-platform IO library"
	HOMEPAGE_URL "https://github.com/cfnptr/mpio"
	LANGUAGES ${MPIO_LANGUAGES})

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED TRUE)

configure_file(cmake/defines.h.in mpio/defines.h)

set(MPIO_SOURCES source/directory.c)

if (CMAKE_SYSTEM_NAME MATCHES "Darwin")
	list(APPEND MPIO_SOURCES source/directory.m)
endif ()

add_library(mpio-static STATIC
	${MPIO_SOURCES})
set_target_properties(mpio-static PROPERTIES
	OUTPUT_NAME "mpio")
target_include_directories(mpio-static PUBLIC
	${PROJECT_BINARY_DIR}
	${PROJECT_SOURCE_DIR}/include)

if (MPIO_BUILD_SHARED)
	add_library(mpio-shared SHARED
		${MPIO_SOURCES})
	set_target_properties(mpio-shared PROPERTIES
		OUTPUT_NAME "mpio"
		WINDOWS_EXPORT_ALL_SYMBOLS ON)
	target_include_directories(mpio-shared PUBLIC
		${PROJECT_BINARY_DIR}
		${PROJECT_SOURCE_DIR}/include)
endif ()
