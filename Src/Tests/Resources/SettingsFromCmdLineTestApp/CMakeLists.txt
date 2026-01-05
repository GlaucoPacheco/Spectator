#
# Copyright (C) 2025 Glauco Pacheco <glaucopacheco@gmail.com>
# All rights reserved.
#
qt_add_executable(SettingsFromCmdLineTestApp)
target_sources(SettingsFromCmdLineTestApp PRIVATE main.cpp)
find_package(Qt6 COMPONENTS Core Concurrent REQUIRED)
target_include_directories(SettingsFromCmdLineTestApp PRIVATE ../../../Lib/)
target_link_libraries(SettingsFromCmdLineTestApp
    PRIVATE
    SpectatorObjs
    Qt::Core
    Qt::Concurrent)
