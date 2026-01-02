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

#ifndef SPECTATOR_SCENARIO_H
#define SPECTATOR_SCENARIO_H

#include "MacroHelpers.h"
#include <QByteArrayView>
#include <QtTypes>
#include <QtClassHelperMacros>
#include <functional>

namespace Spectator
{

class Scenario
{
    Q_DISABLE_COPY_MOVE(Scenario)
public:
    using F = void(*)();
    Scenario(QByteArrayView sourceFile, qint32 sourceLine, QByteArrayView scenarioName, F f);
    ~Scenario() = default;

private:
    const QByteArrayView m_sourceFile;
    const qint32 m_sourceLine;
    const QByteArrayView m_scenarioName;
    const std::function<void()> m_scenarioFunction;
};

}

#define SCENARIO(SCENARIO_NAME) static void _SPECTATOR_CONCATENATE_(_spectator_scenario_fcn_, __LINE__)(); \
static ::Spectator::Scenario _SPECTATOR_CONCATENATE_(_spectator_scenario, __LINE__)(__FILE__, __LINE__, "Scenario: " SCENARIO_NAME, _SPECTATOR_CONCATENATE_(_spectator_scenario_fcn_, __LINE__)); \
static void _SPECTATOR_CONCATENATE_(_spectator_scenario_fcn_, __LINE__)()

#endif // SPECTATOR_SCENARIO_H
