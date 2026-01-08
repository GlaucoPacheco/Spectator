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

#include "Require.h"
#include "SpectatorException.h"
#include "ScenarioRunner.h"
#include "NoDestroy.h"
#include <QByteArray>
#include <QtLogging>
#include <QDebug>
#include <atomic>

namespace Spectator
{

static std::atomic<qsizetype> & globalSuccessfulRequireCounter()
{
    static constinit NoDestroy<std::atomic<qsizetype>> counter{0};
    return counter();
}

static std::atomic<qsizetype> & globalUnsuccessfulRequireCounter()
{
    static constinit NoDestroy<std::atomic<qsizetype>> counter{0};
    return counter();
}

void Require::require(bool expr, QStringView exprAsString, QStringView sourceFile, qint32 sourceLine)
{
    if (expr) [[likely]]
    {
        if (ScenarioRunner::hasCurrent()) [[likely]]
            ScenarioRunner::current().incrementSuccessfulRequireCounter();
        else [[unlikely]]
            ++globalSuccessfulRequireCounter();
    }
    else [[unlikely]]
    {
        ++globalUnsuccessfulRequireCounter();
        QString failureMessage = QString().append("REQUIRE(")
                                          .append(exprAsString).append(u") failed at file://")
                                          .append(sourceFile).append(':').append(QByteArray::number(sourceLine))
                                          .append('.');
        if (ScenarioRunner::hasCurrent()) [[likely]]
            throw SpectatorException(failureMessage);
        else [[unlikely]]
            qFatal() << failureMessage;
    }
}

qsizetype Require::globalSuccessfulRequireCount()
{
    return globalSuccessfulRequireCounter();
}

void Require::resetGlobalCounters()
{
    globalSuccessfulRequireCounter() = 0;
    globalUnsuccessfulRequireCounter() = 0;
}

}
