// Copyright (C) 2026 Glauco Pacheco <glaucopacheco@gmail.com>
// SPDX-License-Identifier: AGPL-3.0-only OR MPL-2.0-no-copyleft-exception

#include "Scenario.h"
#include "ScenarioRepository.h"

using namespace Qt::StringLiterals;

namespace Spectator
{

Scenario::Scenario(QStringView sourceFile, qint32 sourceLine, QStringView scenarioName) :
    Section(sourceFile, sourceLine, scenarioName)
{
    ScenarioRepository::global().addScenario(this);
}

void Scenario::processFailedRequire(const std::source_location location)
{
    QString failureMessage = QString().append(u"REQUIRE failed at file://"_s)
                                          .append(QString::fromUtf8(location.file_name()))
                                          .append(':').append(QString::number(location.line()))
                                          .append('.');
    m_scenarioRunner.incrementUnsuccessfulRequireCounter(failureMessage);
}

}
