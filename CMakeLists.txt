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
# Set compiler options
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-DNDEBUG -O2")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELEASE} ${CMAKE_CXX_FLAGS_DEBUG}")

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pipe")
    add_link_options(-pipe)
    if (CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64")
        set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -ftls-model=initial-exec -march=x86-64-v3 -mavx2")
        add_link_options(-ftls-model=initial-exec)
    endif()
endif()

add_subdirectory(Src)
