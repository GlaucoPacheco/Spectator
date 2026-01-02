#
# Copyright (C) 2025 Glauco Pacheco <glaucopacheco@gmail.com>
# All rights reserved.
#
qt_add_executable(SettingsFromCmdLineTestApp)
target_sources(SettingsFromCmdLineTestApp PRIVATE main.cpp $<TARGET_OBJECTS:SpectatorObjs>)
find_package(Qt6 COMPONENTS Core REQUIRED)
target_include_directories(SettingsFromCmdLineTestApp PRIVATE ../../../Lib/)
target_link_libraries(SettingsFromCmdLineTestApp
    PRIVATE
    Spectator
    Qt::Core)
