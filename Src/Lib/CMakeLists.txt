# Copyright (C) 2026 Glauco Pacheco <glaucopacheco@gmail.com>
# SPDX-License-Identifier: BSD-3-Clause

qt_add_library(SpectatorObjs OBJECT
    Generator.cpp
    Generator.h
    GlobalScopeData.cpp
    GlobalScopeData.h
    MacroHelpers.h
    NoDestroy.h
    Scenario.cpp
    Scenario.h
    ScenarioFilter.h
    ScenarioPathRunResults.cpp
    ScenarioPathRunResults.h
    ScenarioRepository.cpp
    ScenarioRepository.h
    ScenarioRunner.cpp
    ScenarioRunner.h
    ScenarioRunResults.cpp
    ScenarioRunResults.h
    ScenariosRunner.cpp
    ScenariosRunner.h
    ScenariosRunnerPrivate.cpp
    ScenariosRunnerPrivate.h
    Section.cpp
    Section.h
    SectionGuard.cpp
    SectionGuard.h
    Settings.cpp
    Settings.h
    Spectator.cpp
    Spectator.h
    SpectatorException.h
    SpectatorGlobals.h)

find_package(Qt6 COMPONENTS Core Concurrent REQUIRED)
target_link_libraries(SpectatorObjs
    PRIVATE
    Qt::Core
    Qt::Concurrent)
qt_add_library(Spectator SHARED $<TARGET_OBJECTS:SpectatorObjs>)
target_compile_definitions(Spectator PRIVATE SPECTATOR_LIBRARY)
set_target_properties(Spectator PROPERTIES
    C_VISIBILITY_PRESET hidden
    CXX_VISIBILITY_PRESET hidden)
target_link_libraries(Spectator
    PRIVATE
    Qt::Core
    Qt::Concurrent)
