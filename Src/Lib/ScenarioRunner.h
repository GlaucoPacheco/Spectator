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
#include <QSet>
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
    inline void incrementSuccessfullRequireCounter() {++m_successfullRequireCounter;}
    static ScenarioRunner & current();
    ScenarioRunResults runScenario();

private:
    inline void reset()
    {
        m_state = State::HeadingForLeaf;
        m_scenarioRunResults = {};
        m_sectionsStack.clear();
        m_generatorsStack.clear();
        m_sectionsWithFullyVisitedChildren.clear();
        m_currentSectionHasUnvisitedChildren = false;
        m_hasVisitedAllLeafNodes = false;
        m_elapsedTimer.start();
        m_successfullRequireCounter = 0;
    }
    void runScenarioPath();
    inline bool hasVisitedAllLeafNodes() {return m_hasVisitedAllLeafNodes;}

private:
    static constinit thread_local ScenarioRunner * m_pCurrentRunner;
    const Scenario & m_scenario;
    enum class State {HeadingForLeaf, HeadingForRoot};
    State m_state = State::HeadingForLeaf;
    ScenarioRunResults m_scenarioRunResults;
    QStack<Section const *> m_sectionsStack;
    QStack<Generator const *> m_generatorsStack;
    QSet<Section const *> m_sectionsWithFullyVisitedChildren;
    bool m_currentSectionHasUnvisitedChildren = false;
    bool m_hasVisitedAllLeafNodes = false;
    QElapsedTimer m_elapsedTimer;
    qsizetype m_successfullRequireCounter = 0;

};

}

#endif // SPECTATOR_SCENARIO_RUNNER_H
