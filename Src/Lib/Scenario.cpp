// Copyright (C) 2026 Glauco Pacheco <glaucopacheco@gmail.com>
// SPDX-License-Identifier: AGPL-3.0-only OR MPL-2.0-no-copyleft-exception

#include "Scenario.h"
#include "ScenarioRepository.h"
#include "ScenarioRunner.h"
#include "SpectatorException.h"

using namespace Qt::StringLiterals;

namespace Spectator
{

Scenario::Scenario(QStringView sourceFile, qint32 sourceLine, QStringView scenarioName) :
    Section(sourceFile, sourceLine, scenarioName)
{
    ScenarioRepository::global().addScenario(this);
}

Scenario::~Scenario() = default;

void Scenario::REQUIRE(bool expr, const std::source_location location)
{
    if (expr) [[likely]]
        m_pScenarioRunner->incrementSuccessfulRequireCounter();
    else [[unlikely]]
        processFailedRequire(location);
}

void Scenario::INFO(QString message)
{
    m_pScenarioRunner->recordInfoMessage(message);
}

void Scenario::FAIL(QString message, const std::source_location location)
{
    throw SpectatorException(message, QString::fromUtf8(location.file_name()), location.line());
}

ScenarioRunner * Scenario::scenarioRunner()
{
    return m_pScenarioRunner;
}

void Scenario::processFailedRequire(const std::source_location location)
{
    QString failureMessage = QString().append(u"REQUIRE failed at file://"_s)
                                          .append(QString::fromUtf8(location.file_name()))
                                          .append(':').append(QString::number(location.line()))
                                          .append('.');
    m_pScenarioRunner->incrementUnsuccessfulRequireCounter(failureMessage);
}

}
