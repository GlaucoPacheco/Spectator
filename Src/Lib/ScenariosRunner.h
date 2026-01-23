// Copyright (C) 2026 Glauco Pacheco <glaucopacheco@gmail.com>
// SPDX-License-Identifier: AGPL-3.0-only OR MPL-2.0-no-copyleft-exception

#ifndef SPECTATOR_SCENARIOS_RUNNER_H
#define SPECTATOR_SCENARIOS_RUNNER_H

#include "Scenario.h"
#include "ScenarioRunResults.h"
#include "Section.h"
#include <QtClassHelperMacros>
#include <QVector>
#include <QThreadPool>
#include <QFutureWatcher>
#include <QObject>
#include <QtTypes>
#include <QMap>
#include <memory>

namespace Spectator
{

class ScenariosRunner : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(ScenariosRunner)
public:
    ScenariosRunner() = default;
    ~ScenariosRunner() = default;
    void runScenarios();

private slots:
    void onFinishedRunningScenario();

private:
    QVector<Scenario*> fetchScenarios();
    void processScenariosResults();
    void printResults();

private:
    QThreadPool m_threadPool;
    QVector<std::shared_ptr<QFutureWatcher<ScenarioRunResults>>> m_scenarioWatchers;
    QMap<Section const *, ScenarioPathRunResults> m_results;
    qsizetype m_finishedRunningScenariosCounter = 0;
};

}

#endif // SPECTATOR_SCENARIOS_RUNNER_H
