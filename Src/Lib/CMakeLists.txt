#
# Copyright (C) 2025 Glauco Pacheco <glaucopacheco@gmail.com>
# All rights reserved.
#
qt_add_library(SpectatorObjs OBJECT
    Generator.cpp
    Generator.h
    MacroHelpers.h
    NoDestroy.h
    Require.cpp
    Require.h
    Scenario.cpp
    Scenario.h
    ScenarioRepository.cpp
    ScenarioRepository.h
    ScenarioRunner.cpp
    ScenarioRunner.h
    ScenarioRunResults.cpp
    ScenarioRunResults.h
    ScenariosRunner.cpp
    ScenariosRunner.h
    Section.cpp
    Section.h
    SectionGuard.cpp
    SectionGuard.h
    Settings.cpp
    Settings.h
    SpectatorException.h
    SpectatorGlobals.h)

find_package(Qt6 COMPONENTS Core Concurrent REQUIRED)
target_link_libraries(SpectatorObjs
    PRIVATE
    Qt::Core
    Qt::Concurrent)
qt_add_library(Spectator SHARED $<TARGET_OBJECTS:SpectatorObjs>)
target_link_libraries(Spectator
    PRIVATE
    Qt::Core
    Qt::Concurrent)
