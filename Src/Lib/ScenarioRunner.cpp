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

#include "ScenarioRunner.h"
#include <QtLogging>

namespace Spectator
{

constinit thread_local ScenarioRunner * ScenarioRunner::m_pCurrentRunner = nullptr;

ScenarioRunner::ScenarioRunner(const Scenario & scenario) :
    m_scenario(scenario)
{
}

bool ScenarioRunner::tryPushSection(Section const * const pSection)
{
    switch (m_state)
    {
        case State::HeadingForLeaf:
            if (m_sectionsWithFullyVisitedChildren.contains(pSection))
                return false;
            else
            {
                m_sectionsStack.push(pSection);
                return true;
            }
        case State::HeadingForRoot:
            m_currentSectionHasUnvisitedChildren = m_currentSectionHasUnvisitedChildren
                                                   || !m_sectionsWithFullyVisitedChildren.contains(pSection);
            return false;
    }
}

void ScenarioRunner::popSection(Section const * const pSection)
{
    assert(m_sectionsStack.front() == pSection);
    if (m_state == State::HeadingForLeaf)
    {
        m_state = State::HeadingForRoot;
        m_currentSectionHasUnvisitedChildren = false;
    }
    if (!m_currentSectionHasUnvisitedChildren)
        m_sectionsWithFullyVisitedChildren.insert(pSection);
    m_sectionsStack.pop();
    m_hasVisitedAllLeafNodes = m_sectionsStack.isEmpty() && !m_currentSectionHasUnvisitedChildren;
    m_currentSectionHasUnvisitedChildren = false;
}

qsizetype ScenarioRunner::getGeneratorIndex(Generator const * const generator)
{
}

ScenarioRunner & ScenarioRunner::current()
{
    if (m_pCurrentRunner)
        return *m_pCurrentRunner;
    else
        qFatal("Failed to fetch current scenario runner. No scenario runner has been set for this thread.");
}

ScenarioRunResults ScenarioRunner::runScenario()
{
    if (m_pCurrentRunner != nullptr) [[unlikely]]
        qFatal("Failed to set current scenario runner. There is another scenario being ran on this thread and only one scenario can be run at a time per thread.");
    else [[likely]]
        m_pCurrentRunner = this;
    reset();
    do
    {
        m_scenario.scenarioFunction()();
    } while (!hasVisitedAllLeafNodes());
    m_pCurrentRunner = nullptr;
    return m_scenarioRunResults;
}

}
