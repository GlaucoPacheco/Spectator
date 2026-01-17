// Copyright (C) 2026 Glauco Pacheco <glaucopacheco@gmail.com>
// SPDX-License-Identifier: AGPL-3.0-only OR MPL-2.0-no-copyleft-exception

#ifndef SPECTATOR_H
#define SPECTATOR_H

#include "MacroHelpers.h"
#include "Generator.h"
#include "Scenario.h"
#include "ScenarioRunner.h"
#include "ScenariosRunner.h"
#include "SpectatorException.h"
#include "Section.h"
#include "SectionGuard.h"
#include <QCoreApplication>
#include <QString>
#include <source_location>

#define SPECTATOR_MAIN \
    int main(int argc, char ** argv) \
    { \
        QCoreApplication app(argc, argv); \
        ::Spectator::ScenariosRunner scenariosRunner; \
        scenariosRunner.runScenarios(); \
        return QCoreApplication::exec(); \
    }

#define SECTION(NAME) \
    static const ::Spectator::Section _SPECTATOR_CONCATENATE_(_spectator_section, __LINE__)(_SPECTATOR_TO_UTF_16_STRING_LITERAL(__FILE__), __LINE__, _SPECTATOR_TO_UTF_16_STRING_LITERAL(NAME)); \
    if (::Spectator::SectionGuard _SPECTATOR_CONCATENATE_(_spectator_section_guard, __LINE__)(& _SPECTATOR_CONCATENATE_(_spectator_section, __LINE__), this); _SPECTATOR_CONCATENATE_(_spectator_section_guard, __LINE__).hasEntered())
#define GIVEN(NAME) SECTION("Given: " NAME)
#define WHEN(NAME) SECTION("When: " NAME)
#define THEN(NAME) SECTION("Then: " NAME)
#define AND_WHEN(NAME) SECTION("And When: " NAME)
#define AND_THEN(NAME) SECTION("And Then: " NAME)

namespace Spectator
{
    void REQUIRE(bool expr, const std::source_location location = std::source_location::current());
    void INFO(QString message);
    void FAIL(QString message, const std::source_location location = std::source_location::current());
}

#endif // SPECTATOR_H
