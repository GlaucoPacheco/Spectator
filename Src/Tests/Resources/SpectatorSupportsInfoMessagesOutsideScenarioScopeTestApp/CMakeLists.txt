#
# Copyright (C) 2026 Glauco Pacheco <glaucopacheco@gmail.com>
# All rights reserved.
#
qt_add_executable(SpectatorSupportsInfoMessagesOutsideScenarioScopeTestApp)
target_sources(SpectatorSupportsInfoMessagesOutsideScenarioScopeTestApp PRIVATE
    main.cpp)
find_package(Qt6 COMPONENTS Core Concurrent REQUIRED)
target_include_directories(SpectatorSupportsInfoMessagesOutsideScenarioScopeTestApp PRIVATE ../../../Lib/)
target_link_libraries(SpectatorSupportsInfoMessagesOutsideScenarioScopeTestApp
    PRIVATE
    SpectatorObjs
    Qt::Core
    Qt::Concurrent)
