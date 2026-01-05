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

#include "ScenarioRepository.h"
#include "NoDestroy.h"
#include <QMutexLocker>
#include <QtLogging>

namespace Spectator
{

void ScenarioRepository::addScenario(Scenario const * const pScenario)
{
    assert(pScenario);
    QMutexLocker locker(&m_scenariosLock);
    if (!m_scenarios.contains(pScenario)) [[likely]]
        m_scenarios.append(pScenario);
    else [[unlikely]]
        qFatal("Failed to add scenario to repository. Scenario has already been added and scenarios can only be added once.");
}

qsizetype ScenarioRepository::size()
{
    QMutexLocker locker(&m_scenariosLock);
    return m_scenarios.size();
}

const Scenario & ScenarioRepository::operator[](qsizetype index)
{
    assert(0 <= index && index < m_scenarios.size());
    QMutexLocker locker(&m_scenariosLock);
    return *m_scenarios[index];
}

ScenarioRepository & ScenarioRepository::global()
{
    static NoDestroy<ScenarioRepository*> instance(new ScenarioRepository);
    static NoDestroyPtrDeleter<ScenarioRepository*> instanceDeleter(instance);
    auto *pInstance = instance();
    if (pInstance) [[likely]]
        return *pInstance;
    else [[unlikely]]
        qFatal("Failed to fetch global instance of scenario repository. Program is terminating and the instance has already been destroyed.");
}

}
