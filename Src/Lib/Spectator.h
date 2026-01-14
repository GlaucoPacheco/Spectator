// SPDX-License-Identifier: CDDL-1.0
//
// CDDL HEADER START
//
// The contents of this file are subject to the terms of the
// Common Development and Distribution License 1.0 (the "License").
// You may not use this file except in compliance with the License.
//
// You can obtain a copy of the license at usr/src/OPENSOLARIS.LICENSE
// or https://opensource.org/licenses/CDDL-1.0.
// See the License for the specific language governing permissions
// and limitations under the License.
//
// When distributing Covered Code, include this CDDL HEADER in each
// file and include the License file at usr/src/OPENSOLARIS.LICENSE.
// If applicable, add the following below this CDDL HEADER, with the
// fields enclosed by brackets "[]" replaced with your own identifying
// information: Portions Copyright [yyyy] [name of copyright owner]
//
// CDDL HEADER END
//

//
// Copyright (C) 2025 Glauco Pacheco <glaucopacheco@gmail.com>
//

#ifndef SPECTATOR_H
#define SPECTATOR_H

#include "MacroHelpers.h"
#include "Generator.h"
#include "Require.h"
#include "Scenario.h"
#include "ScenarioRunner.h"
#include "ScenariosRunner.h"
#include "SpectatorException.h"
#include "Section.h"
#include "SectionGuard.h"
#include <QCoreApplication>

#define SPECTATOR_MAIN \
    int main(int argc, char ** argv) \
    { \
        QCoreApplication app(argc, argv); \
        ::Spectator::ScenariosRunner scenariosRunner; \
        scenariosRunner.runScenarios(); \
        return QCoreApplication::exec(); \
    }

#define INFO(MSG) ::Spectator::ScenarioRunner::recordInfoMessage(MSG)
#define FAIL(MSG) throw ::Spectator::SpectatorException(MSG, __FILE__, __LINE__)
#define SECTION(NAME) \
    static const ::Spectator::Section _SPECTATOR_CONCATENATE_(_spectator_section, __LINE__)(_SPECTATOR_TO_UTF_16_STRING_LITERAL(__FILE__), __LINE__, _SPECTATOR_TO_UTF_16_STRING_LITERAL(NAME)); \
    if (::Spectator::SectionGuard _SPECTATOR_CONCATENATE_(_spectator_section_guard, __LINE__)(& _SPECTATOR_CONCATENATE_(_spectator_section, __LINE__)); _SPECTATOR_CONCATENATE_(_spectator_section_guard, __LINE__).hasEntered())
#define GIVEN(NAME) SECTION("Given: " NAME)
#define WHEN(NAME) SECTION("When: " NAME)
#define THEN(NAME) SECTION("Then: " NAME)
#define AND_WHEN(NAME) SECTION("And When: " NAME)
#define AND_THEN(NAME) SECTION("And Then: " NAME)

#endif // SPECTATOR_H
