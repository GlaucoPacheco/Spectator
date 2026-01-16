// Copyright (C) 2026 Glauco Pacheco <glaucopacheco@gmail.com>
// SPDX-License-Identifier: AGPL-3.0-only OR CDDL-1.0

#include "Scenario.h"
#include "ScenarioRepository.h"

namespace Spectator
{

Scenario::Scenario(QStringView sourceFile, qint32 sourceLine, QStringView scenarioName, F f) :
    Section(sourceFile, sourceLine, scenarioName),
    m_scenarioFunction(f)
{
    ScenarioRepository::global().addScenario(this);
}

}
