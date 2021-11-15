cmake_minimum_required(VERSION 3.10)

if (${CMAKE_SOURCE_DIR} MATCHES ${CMAKE_BINARY_DIR})
	message(FATAL_ERROR "Prevented in-tree build")
endif ()

if (TARGET mpio)
	return()
endif ()

project(mpio
	VERSION 1.0.0
	DESCRIPTION "Multi-platform IO library"
	HOMEPAGE_URL "https://github.com/cfnptr/mpio"
	LANGUAGES C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED TRUE)

configure_file(
	cmake/defines.h.in
	mpio/defines.h)

add_library(mpio INTERFACE)

target_include_directories(mpio PUBLIC
	${PROJECT_BINARY_DIR}
	${PROJECT_SOURCE_DIR}/include)
