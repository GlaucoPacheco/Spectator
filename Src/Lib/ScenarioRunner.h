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

#ifndef SPECTATOR_SCENARIO_RUNNER_H
#define SPECTATOR_SCENARIO_RUNNER_H

#include "Scenario.h"
#include "ScenarioRunResults.h"
#include <QStack>
#include <QElapsedTimer>
#include <QtClassHelperMacros>

namespace Spectator
{

class Section;
class Generator;

class ScenarioRunner
{
    Q_DISABLE_COPY_MOVE(ScenarioRunner)
public:
    ScenarioRunner(const Scenario & scenario);
    ~ScenarioRunner() = default;
    bool tryPushSection(Section const * const pSection);
    void popSection(Section const * const pSection);
    qsizetype getGeneratorIndex(Generator const * const generator);
    inline void incrementSuccessfullRequireCount() {++m_successfullRequireCount;}
    static ScenarioRunner & current();
    ScenarioRunResults runScenario();

private:
    bool hasVisitedAllLeafNodes();

private:
    static thread_local ScenarioRunner * m_pCurrentRunner;
    const Scenario & m_scenario;
    ScenarioRunResults m_scenarioRunResults;
    QStack<Section*> m_sectionsStack;
    QStack<Generator*> m_generatorsStack;
    QElapsedTimer m_elapsedTimer;
    qsizetype m_successfullRequireCount = 0;

};

}

#endif // SPECTATOR_SCENARIO_RUNNER_H
