# Copyright (C) 2026 Glauco Pacheco <glaucopacheco@gmail.com>
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.19 FATAL_ERROR)

# Set project options
project(Spectator)
enable_language(CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
include(CheckPIESupported)
check_pie_supported()
set (CMAKE_POSITION_INDEPENDENT_CODE True)
include(CheckIPOSupported)
check_ipo_supported(RESULT HAS_IPO)
if (HAS_IPO AND (NOT (CMAKE_BUILD_TYPE STREQUAL "Debug")))
    message("Enabling IPO.")
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION True)
else()
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION False)
endif()
# Set Qt options
find_package(Qt6 REQUIRED COMPONENTS Core)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_C_VISIBILITY_PRESET hidden)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
# Set compiler options
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-DNDEBUG -O2")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELEASE} ${CMAKE_CXX_FLAGS_DEBUG}")

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pipe")
    add_link_options(-pipe)
    if (CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64" AND (NOT (CMAKE_BUILD_TYPE STREQUAL "Debug")))
        message("Enabling -ftls-model=initial-exec -march=x86-64-v3 -mavx2.")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ftls-model=initial-exec -march=x86-64-v3 -mavx2")
        add_link_options(-ftls-model=initial-exec)
    endif()
endif()
find_package(Git REQUIRED)
if(Git_FOUND)
  execute_process(
    COMMAND ${GIT_EXECUTABLE} --no-pager log -n 1 HEAD --pretty=format:'%h-%ct'
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    OUTPUT_VARIABLE CONFIGURATION_VERSION_STRING_RAW
    RESULT_VARIABLE result
    ERROR_QUIET
    OUTPUT_STRIP_TRAILING_WHITESPACE)
  if(NOT result EQUAL 0)
    message(FATAL_ERROR "Failed to fetch Git short hash and UNIX timestamp.")
  endif()
  string(REPLACE "'" "" CONFIGURATION_VERSION_STRING "${CONFIGURATION_VERSION_STRING_RAW}")
  message(STATUS "Configuration version: ${CONFIGURATION_VERSION_STRING}")
endif()

add_subdirectory(Src)
