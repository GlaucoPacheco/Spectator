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
#include "SpectatorException.h"
#include "Generator.h"
#include "NoDestroy.h"
#include <QtLogging>
#include <QDebug>
#include <QElapsedTimer>
#include <QString>
#include <QByteArray>
#include <QMutex>
#include <QMutexLocker>
#include <Qt>
#include <QTextStream>
#include <atomic>
#include <exception>

using namespace Qt::StringLiterals;

namespace Spectator
{

static std::atomic<qsizetype> & globalSuccessfulRequireCounter()
{
    static constinit NoDestroy<std::atomic<qsizetype>> counter{0};
    return counter();
}

static QMutex & globalInfoMessagesLock()
{
    NoDestroy<QMutex> lock;
    return lock();
}

static QSet<QString> * globalInfoMessages()
{
    static NoDestroy<QSet<QString>*> pInstance{new QSet<QString>};
    static NoDestroyPtrDeleter<QSet<QString>*> instanceDeleter(pInstance);
    return pInstance();
}

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

void ScenarioRunner::incrementSuccessfulRequireCounter()
{
    if (hasCurrent())
        ++m_pCurrentRunner->m_successfulRequireCounter;
    else
        ++globalSuccessfulRequireCounter();
}

void ScenarioRunner::incrementUnsuccessfulRequireCounter(QString failureMessage)
{
    if (hasCurrent())
    {
        ++m_pCurrentRunner->m_unsuccessfulRequireCounter;
        throw SpectatorException(failureMessage);
    }
    else
        qFatal() << failureMessage << Qt::endl;
}

void ScenarioRunner::addInfoMessage(QString message)
{
    if (hasCurrent())
        m_pCurrentRunner->m_infoMessages[m_pCurrentRunner->m_sectionsStack.top()].insert(message);
    else
    {
        QMutexLocker locker(&globalInfoMessagesLock());
        auto * pInfoMessages = globalInfoMessages();
        if (pInfoMessages)
            pInfoMessages->insert(message);
        else
            qInfo() << u"INFO: "_s << message << Qt::endl;
    }
}

ScenarioRunResults ScenarioRunner::runScenario(const Scenario & scenario)
{
    ScenarioRunner scenarioRunner(scenario);
    if (m_pCurrentRunner != nullptr) [[unlikely]]
        qFatal("Failed to set current scenario runner. There is another scenario being ran on this thread and only one scenario can be run at a time per thread.");
    else [[likely]]
        m_pCurrentRunner = &scenarioRunner;
    do
    {
        scenarioRunner.runScenarioPath();
    } while (!scenarioRunner.hasVisitedAllLeafNodes());
    m_pCurrentRunner = nullptr;
    return scenarioRunner.m_scenarioRunResults;
}

void ScenarioRunner::printGlobalStats(QString & buffer)
{
    QTextStream outputStream(&buffer, QIODeviceBase::WriteOnly);
    outputStream << u"Global Scope"_s << Qt::endl;
    QMutexLocker locker(&globalInfoMessagesLock());
    auto *pGlobalInfoMessages = globalInfoMessages();
    if (pGlobalInfoMessages) [[likely]]
    {
        for (const auto & message : *pGlobalInfoMessages)
            outputStream << u"INFO: "_s << message << Qt::endl;
    }
    outputStream << u"Assertions: "_s << globalSuccessfulRequireCounter() << Qt::endl;
}

void ScenarioRunner::runScenarioPath()
{
    QElapsedTimer elapsedTimer;
    elapsedTimer.start();
    qsizetype runCount = 0;
    m_successfulRequireCounter = 0;
    m_state = State::HeadingForLeaf;
    do
    {
        ++runCount;
        m_idxNextGenerator = 0;
        m_isValidatingGeneratorStack = !m_generatorsStack.isEmpty();
        try
        {
            m_scenario.scenarioFunction()();
        }
        catch (const SpectatorException &ex)
        {
            m_failureMessage = ex.message();
        }
        catch (const std::exception &ex)
        {
            m_failureMessage = QString().append(u"\nTest code has thrown an unhandled std::exception with message: "_s)
                                        .append(QString::fromUtf8(ex.what()));
        }
        catch (...)
        {
            m_failureMessage = QString().append(u"\nTest code has thrown an unhandled non-standard exception."_s);
        }
    } while (!hasConsumedAllGeneratorDataOnCurrentPath());
    const auto elapsedTimeInNSecs = elapsedTimer.nsecsElapsed();
    m_scenarioRunResults.addScenarioPath(m_pathToLeafSection,
                                         m_infoMessages,
                                         m_failureMessage,
                                         m_successfulRequireCounter,
                                         m_unsuccessfulRequireCounter,
                                         runCount,
                                         elapsedTimeInNSecs);
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
