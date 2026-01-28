# Copyright (C) 2026 Glauco Pacheco <glaucopacheco@gmail.com>
# SPDX-License-Identifier: BSD-3-Clause

qt_add_executable(SpectatorSupportsTestFilteringTestApp)
target_sources(SpectatorSupportsTestFilteringTestApp PRIVATE
    main.cpp
    scenarios.cpp
    scenario_on_another_file.cpp)
find_package(Qt6 COMPONENTS Core Concurrent REQUIRED)
target_include_directories(SpectatorSupportsTestFilteringTestApp PRIVATE ../../../Lib/)
target_link_libraries(SpectatorSupportsTestFilteringTestApp
    PRIVATE
    SpectatorObjs
    Qt::Core
    Qt::Concurrent)
