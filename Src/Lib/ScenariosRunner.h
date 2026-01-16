// Copyright (C) 2026 Glauco Pacheco <glaucopacheco@gmail.com>
// SPDX-License-Identifier: AGPL-3.0-only OR CDDL-1.0

#ifndef SPECTATOR_SCENARIOS_RUNNER_H
#define SPECTATOR_SCENARIOS_RUNNER_H

#include <QtClassHelperMacros>

namespace Spectator
{

class ScenariosRunner
{
    Q_DISABLE_COPY_MOVE(ScenariosRunner)
public:
    ScenariosRunner() = default;
    ~ScenariosRunner() = default;
    void runScenarios();
};

}

#endif // SPECTATOR_SCENARIOS_RUNNER_H
