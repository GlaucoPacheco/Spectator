// Copyright (C) 2026 Glauco Pacheco <glaucopacheco@gmail.com>
// SPDX-License-Identifier: AGPL-3.0-only OR MPL-2.0-no-copyleft-exception

#include "ScenariosRunner.h"
#include <QCoreApplication>
#include <QtLogging>

namespace Spectator
{

void ScenariosRunner::runScenarios()
{
    auto *pApp = QCoreApplication::instance();
    if (!pApp) [[unlikely]]
        qFatal("Failed to run scenarios. Current QCoreApplication instance is null. Please, create a QCoreInstance before trying to run the scenarios.");    
}

}
