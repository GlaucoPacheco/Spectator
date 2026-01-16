// Copyright (C) 2026 Glauco Pacheco <glaucopacheco@gmail.com>
// SPDX-License-Identifier: AGPL-3.0-only OR CDDL-1.0

#include "Require.h"
#include "ScenarioRunner.h"
#include <QString>

using namespace Qt::StringLiterals;

namespace Spectator
{

void Require::require(bool expr, QStringView exprAsString, QStringView sourceFile, qint32 sourceLine)
{
    if (expr) [[likely]]
        ScenarioRunner::incrementSuccessfulRequireCounter();
    else [[unlikely]]
    {
        QString failureMessage = QString().append(u"REQUIRE("_s)
                                          .append(exprAsString).append(u") failed at file://"_s)
                                          .append(sourceFile).append(':').append(QString::number(sourceLine))
                                          .append('.');
        ScenarioRunner::incrementUnsuccessfulRequireCounter(failureMessage);
    }
}

}
