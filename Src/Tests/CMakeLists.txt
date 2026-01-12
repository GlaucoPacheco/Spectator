#
# Copyright (C) 2025 Glauco Pacheco <glaucopacheco@gmail.com>
# All rights reserved.
#
add_subdirectory(Resources)
qt_add_executable(Tests)
target_sources(Tests PRIVATE main.cpp)
find_package(Qt6 COMPONENTS Core Concurrent REQUIRED)
add_dependencies(Tests 
    SettingsFromCmdLineTestApp
    SpectatorStoresScenariosTestApp
    SpectatorSupportsInfoMessagesOutsideScenarioScopeTestApp)
target_link_libraries(Tests
    PRIVATE
    Spectator
    Qt::Core
    Qt::Concurrent)
