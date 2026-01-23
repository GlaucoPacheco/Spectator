// Copyright (C) 2026 Glauco Pacheco <glaucopacheco@gmail.com>
// SPDX-License-Identifier: AGPL-3.0-only OR MPL-2.0-no-copyleft-exception

#include "ScenariosRunner.h"
#include "ScenarioRepository.h"
#include "Scenario.h"
#include "ScenarioRunResults.h"
#include <QCoreApplication>
#include <QtLogging>
#include <QtConcurrent>
#include <QFuture>
#include <QDeadlineTimer>
#include <QEventLoop>
#include <QTimer>

namespace Spectator
{

void ScenariosRunner::runScenarios()
{
    auto *pApp = QCoreApplication::instance();
    if (!pApp) [[unlikely]]
        qFatal("Failed to run scenarios. Current QCoreApplication instance is null. Please, create a QCoreInstance before trying to run the scenarios.");
    auto scenarios = fetchScenarios();
    if (scenarios.isEmpty())
        qFatal("Failed to run scenarios. There are no scenarios to run.");
    m_scenarioWatchers.resize(scenarios.size());
    for (auto i = 0; i < scenarios.size(); ++i)
    {
        m_scenarioWatchers[i].reset(new QFutureWatcher<ScenarioRunResults>{});
        QObject::connect(m_scenarioWatchers[i].get(), &QFutureWatcher<ScenarioRunResults>::finished, this, &ScenariosRunner::onFinishedRunningScenario, Qt::QueuedConnection);
        auto * pScenario = scenarios[i];
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
        qFatal("Finished running scenarios.");
    }
}

QVector<Scenario*> ScenariosRunner::fetchScenarios()
{
    return ScenarioRepository::global().getAll();
}

}
