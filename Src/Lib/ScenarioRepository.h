// Copyright (C) 2026 Glauco Pacheco <glaucopacheco@gmail.com>
// SPDX-License-Identifier: AGPL-3.0-only OR MPL-2.0-no-copyleft-exception

#ifndef SPECTATOR_SCENARIO_REPOSITORY_H
#define SPECTATOR_SCENARIO_REPOSITORY_H

#include "Scenario.h"
#include <QMutex>
#include <QVector>
#include <QtClassHelperMacros>

namespace Spectator
{

class ScenarioRepository
{
    Q_DISABLE_COPY_MOVE(ScenarioRepository)
public:
    ~ScenarioRepository() = default;
    void addScenario(Scenario * pScenario);
    qsizetype size();
    Scenario & operator[](qsizetype index);
    static ScenarioRepository & global();

private:
    ScenarioRepository() = default;

private:
    QMutex m_scenariosLock;
    QVector<Scenario*> m_scenarios;
};

}

#endif // SPECTATOR_SCENARIO_REPOSITORY_H
