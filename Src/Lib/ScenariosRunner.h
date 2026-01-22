// Copyright (C) 2026 Glauco Pacheco <glaucopacheco@gmail.com>
// SPDX-License-Identifier: AGPL-3.0-only OR MPL-2.0-no-copyleft-exception

#ifndef SPECTATOR_SCENARIOS_RUNNER_H
#define SPECTATOR_SCENARIOS_RUNNER_H

#include "Scenario.h"
#include <QtClassHelperMacros>
#include <QVector>
#include <QThreadPool>

namespace Spectator
{

class ScenariosRunner
{
    Q_DISABLE_COPY_MOVE(ScenariosRunner)
public:
    ScenariosRunner() = default;
    ~ScenariosRunner() = default;
    void runScenarios();

private:
    QVector<Scenario*> fetchScenarios();
    QThreadPool m_threadPool;
};

}

#endif // SPECTATOR_SCENARIOS_RUNNER_H
