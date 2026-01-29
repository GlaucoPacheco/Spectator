// Copyright (C) 2026 Glauco Pacheco <glaucopacheco@gmail.com>
// SPDX-License-Identifier: AGPL-3.0-only OR MPL-2.0-no-copyleft-exception

#include "ScenariosRunner.h"
#include "ScenarioRepository.h"
#include "Scenario.h"
#include "ScenarioRunResults.h"
#include "Settings.h"
#include "ScenarioFilter.h"
#include <QCoreApplication>
#include <QtLogging>
#include <QtConcurrent>
#include <QFuture>
#include <QDeadlineTimer>
#include <QEventLoop>
#include <QTimer>
#include <QMetaObject>
#include <QString>
#include <Qt>
#include <cstdio>

namespace Spectator
{

void ScenariosRunner::runScenarios()
{
    auto *pApp = QCoreApplication::instance();
    if (!pApp) [[unlikely]]
        qFatal("Failed to run scenarios. Current QCoreApplication instance is null. Please, create a QCoreInstance before trying to run the scenarios.");
    const auto settings = Settings::fromCmdLine();
    m_threadPool.setMaxThreadCount(settings.threadCount());
    auto allScenarios = fetchScenarios();
    ScenarioFilter scenarioFilter(settings);
    QVector<Scenario*> filteredScenarios;
    filteredScenarios.reserve(allScenarios.size());
    for (qsizetype i = 0; i < allScenarios.size(); ++i)
    {
        if (scenarioFilter.hasToRunScenario(allScenarios[i]))
            filteredScenarios.append(allScenarios[i]);
    }
    if (filteredScenarios.isEmpty())
        qFatal("Failed to run scenarios. There are no scenarios to run.");
    m_scenarioWatchers.resize(filteredScenarios.size());
    for (auto i = 0; i < filteredScenarios.size(); ++i)
    {
        m_scenarioWatchers[i].reset(new QFutureWatcher<ScenarioRunResults>{});
        QObject::connect(m_scenarioWatchers[i].get(), &QFutureWatcher<ScenarioRunResults>::finished, this, &ScenariosRunner::onFinishedRunningScenario, Qt::QueuedConnection);
        auto * pScenario = filteredScenarios[i];
        m_scenarioWatchers[i]->setFuture(QtConcurrent::run(&m_threadPool, [pScenario]()
            {
                QEventLoop eventLoop;
                QObject ctxObject;
                ScenarioRunResults results;
                QTimer::singleShot(0, &ctxObject, [&results, &eventLoop, pScenario](){results = ScenarioRunner::runScenario(*pScenario); eventLoop.exit();});
                eventLoop.exec();
                return results;
            }));
    }
}

void ScenariosRunner::onFinishedRunningScenario()
{
    if (++m_finishedRunningScenariosCounter == m_scenarioWatchers.size())
    {
        QDeadlineTimer deadline(5000);
        m_threadPool.waitForDone(deadline);
        if (deadline.hasExpired())
            qFatal("Failed to wait for the threads of the thread pool responsible for running scenarios to stop.");
        processScenariosResults();
        printResults();
        if (!QMetaObject::invokeMethod(QCoreApplication::instance(), &QCoreApplication::quit, Qt::QueuedConnection))
            qFatal("Failed schedule call to QCoreApplication::quit.");
    }
}

QVector<Scenario*> ScenariosRunner::fetchScenarios()
{
    return ScenarioRepository::global().getAll();
}

void ScenariosRunner::processScenariosResults()
{
    for (auto & watcher : m_scenarioWatchers)
    {
        auto scenarioPaths = watcher->result().scenarioPaths();
        for (auto it = scenarioPaths.cbegin(), end = scenarioPaths.cend(); it != end; ++it)
        {
            auto & scenarioPath = m_results[it->pathToLeafSection().top()];
            scenarioPath.pathToLeafSection() = it->pathToLeafSection();
            scenarioPath.infoMessages() = it->infoMessages();
            scenarioPath.fatalMessage() = it->fatalMessage();
            if (it->fatalMessage().isEmpty()) [[likely]]
                ++m_successfulScenarioPathRunCounter;
            else [[unlikely]]
                ++m_unsuccessfulScenarioPathRunCounter;
            scenarioPath.successfulRequireCount() += it->successfulRequireCount();
            m_successfulRequireCounter += it->successfulRequireCount();
            scenarioPath.unsuccessfulRequireCount() += it->unsuccessfulRequireCount();
            m_unsuccessfulRequireCounter += it->unsuccessfulRequireCount();
            scenarioPath.runCount() += it->runCount();
            scenarioPath.elapsedTimeInNSecs() += it->elapsedTimeInNSecs();
            m_elapsedTimeInNSecs += it->elapsedTimeInNSecs();
        }
    }
}

void ScenariosRunner::printResults()
{
    QString buffer;
    buffer.reserve(1ul << 20);
    QTextStream bufferedStream(&buffer, QIODevice::WriteOnly);
    printSuccessfullScenariosPaths(bufferedStream);
    printUnsuccessfullScenariosPaths(bufferedStream);
    printScenarioPathsStats(bufferedStream);
    QTextStream(stdout) << buffer << Qt::endl;
}

void ScenariosRunner::printSuccessfullScenariosPaths(QTextStream & stream)
{
    if (m_successfulScenarioPathRunCounter == 0) [[unlikely]]
        return;
    stream << "\n------------------------------------------\n";
    stream << "Passed scenario paths\n";
    stream << "------------------------------------------\n";
    for (auto it = m_results.cbegin(), end = m_results.cend(); it != end; ++it)
    {
        if (it->fatalMessage().isEmpty()) [[likely]]
        {
            for (const auto & section : it->pathToLeafSection())
            {
                stream << section->name() << Qt::endl;
                const auto & infoMessages = it->infoMessages()[section];
                for (const auto & infoMsg : infoMessages)
                    stream << infoMsg << Qt::endl;
            }
            stream << "Stats: [Time: " << QString::number(it->elapsedTimeInNSecs()/1000000.0, 'g', 3) << "ms; Run count: " << it->runCount() << "; Require count: " << it->successfulRequireCount() << ']' << Qt::endl;
            stream << Qt::endl;
        }
    }
}

void ScenariosRunner::printUnsuccessfullScenariosPaths(QTextStream & stream)
{
    if (m_unsuccessfulScenarioPathRunCounter == 0) [[likely]]
        return;
    stream << "\n------------------------------------------\n";
    stream << "Failed scenario paths\n";
    stream << "------------------------------------------\n";
    for (auto it = m_results.cbegin(), end = m_results.cend(); it != end; ++it)
    {
        if (!it->fatalMessage().isEmpty()) [[likely]]
        {
            for (const auto & section : it->pathToLeafSection())
                stream << section->name() << Qt::endl;
            stream << it->fatalMessage() << Qt::endl;
            stream << "Stats: [Time: " << QString::number(it->elapsedTimeInNSecs()/1000000.0, 'g', 3) << "ms; Run count: " << it->runCount() << "; Successful require count: " << it->successfulRequireCount() << ']' << Qt::endl;
            stream << Qt::endl;
        }
    }
}

void ScenariosRunner::printScenarioPathsStats(QTextStream & stream)
{
    stream << "\n------------------------------------------\n";
    stream << "Scenario paths stats\n";
    stream << "------------------------------------------\n";
    stream << "Total scenarios paths ran: " << m_successfulScenarioPathRunCounter + m_unsuccessfulScenarioPathRunCounter << '\n';
    stream << "Successful scenarios paths ran: " << m_successfulScenarioPathRunCounter << '\n';
    stream << "Unsuccessful scenarios paths ran: " << m_unsuccessfulScenarioPathRunCounter << '\n';
    stream << "Total requires in scenarios paths: " << m_successfulRequireCounter + m_unsuccessfulRequireCounter << '\n';
    stream << "Successful requires in scenarios paths: " << m_successfulRequireCounter << '\n';
    stream << "Unsuccessful requires in scenarios paths: " << m_unsuccessfulRequireCounter << '\n';
    stream << "Time: " << QString::number(m_elapsedTimeInNSecs/1000000.0, 'g', 3) << "ms\n";
    if (m_unsuccessfulScenarioPathRunCounter == 0 && m_unsuccessfulRequireCounter == 0)
        stream << "\nAll " << m_successfulScenarioPathRunCounter << " scenarios paths passed.\n\n";
    else
        stream << "\nThere are " << m_unsuccessfulScenarioPathRunCounter << " failing scenarios paths.\n\n";
}

}
