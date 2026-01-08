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
#include "Generator.h"
#include <QtLogging>
#include <QDebug>
#include <QElapsedTimer>
#include <QString>

using namespace Qt::StringLiterals;

namespace Spectator
{

constinit thread_local ScenarioRunner * ScenarioRunner::m_pCurrentRunner = nullptr;

ScenarioRunner::ScenarioRunner(const Scenario & scenario) :
    m_scenario(scenario)
{
    m_sectionsStack.reserve(16);
    m_generatorsStack.reserve(8);
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
            m_currentPathHasUnvisitedChildren = m_currentPathHasUnvisitedChildren
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
        tryToAdvanceGeneratorsOnCurrentPath();
        m_pathToLeafSection = m_sectionsStack;
        m_currentPathHasUnvisitedChildren = !hasConsumedAllGeneratorDataOnCurrentPath();
    }
    if (!m_currentPathHasUnvisitedChildren)
        m_sectionsWithFullyVisitedChildren.insert(pSection);
    m_sectionsStack.pop();
    m_hasVisitedAllLeafNodes = m_sectionsStack.isEmpty()
                               && hasConsumedAllGeneratorDataOnCurrentPath()
                               && !m_currentPathHasUnvisitedChildren;
}

qsizetype ScenarioRunner::getGeneratorIndex(Generator const * const generator)
{
    if (!m_isValidatingGeneratorStack)
        m_generatorsStack.push({generator, 0});
    else if (m_idxNextGenerator >= m_generatorsStack.size() || m_generatorsStack[m_idxNextGenerator].first != generator)
    {
        qFatal() << QString(u"Generators must be declared in the section's outermost scope "
                             "(the entire section body). Generator at %1:%2 has not."_s)
                             .arg(generator->sourceFile()).arg(generator->sourceLine());
    }
    return m_generatorsStack[m_idxNextGenerator++].second;
}

ScenarioRunner & ScenarioRunner::current()
{
    if (m_pCurrentRunner)
        return *m_pCurrentRunner;
    else
        qFatal("Failed to fetch current scenario runner. No scenario runner has been set for this thread.");
}

bool ScenarioRunner::hasCurrent()
{
    return m_pCurrentRunner != nullptr;
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
        runScenarioPath();
    } while (!hasVisitedAllLeafNodes());
    m_pCurrentRunner = nullptr;
    return m_scenarioRunResults;
}

void ScenarioRunner::runScenarioPath()
{
    QElapsedTimer elapsedTimer;
    elapsedTimer.start();
    qsizetype runCount = 0;
    m_successfullRequireCounter = 0;
    do
    {
        // TODO catch SpectatorException
        ++runCount;
        m_isValidatingGeneratorStack = !m_generatorsStack.isEmpty();
        m_scenario.scenarioFunction()();
    } while (!hasConsumedAllGeneratorDataOnCurrentPath());
    const auto elapsedTimeInNSecs = elapsedTimer.nsecsElapsed();
    m_scenarioRunResults.addScenarioPath(m_pathToLeafSection, m_successfullRequireCounter, runCount, elapsedTimeInNSecs);
}

void ScenarioRunner::tryToAdvanceGeneratorsOnCurrentPath()
{
    m_hasConsumedAllGeneratorDataOnCurrentPath = true;
    if (!m_generatorsStack.isEmpty())
    {
        for (qsizetype idx = (m_generatorsStack.size() - 1); idx >= 0; --idx)
        {
            auto & currentGenerator = m_generatorsStack[idx];
            if (currentGenerator.second < (currentGenerator.first->size() - 1))
            {
                ++currentGenerator.second;
                for (qsizetype idxToReset = (idx + 1); idxToReset < m_generatorsStack.size(); ++idxToReset)
                    m_generatorsStack[idxToReset].second = 0;
                m_hasConsumedAllGeneratorDataOnCurrentPath = false;
                return;
            }
        }
        m_generatorsStack.clear();
    }
}

}
