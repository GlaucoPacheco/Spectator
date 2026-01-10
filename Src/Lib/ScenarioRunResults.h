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

#ifndef SPECTATOR_SCENARIO_RUN_RESULTS_H
#define SPECTATOR_SCENARIO_RUN_RESULTS_H

#include <QStack>
#include <QMap>
#include <QSet>
#include <QString>
#include <QtTypes>

namespace Spectator
{

class Section;

struct ScenarioPathRun
{
    QStack<Section const *> pathToLeafSection;
    QMap<Section const *, QSet<QString>> infoMessages;
    QString fatalMessage;
    qsizetype successfulRequireCount = 0;
    qsizetype unsuccessfulRequireCount = 0;
    qsizetype runCount = 0;
    qint64 elapsedTimeInNSecs = 0;
};

class ScenarioRunResults
{
public:
    ScenarioRunResults() = default;
    ~ScenarioRunResults() = default;
    void addScenarioPath(QStack<Section const *> pathToLeafSection,
                         QMap<Section const *, QSet<QString>> infoMessages,
                         QString fatalMessage,
                         qsizetype successfulRequireCount,
                         qsizetype unsuccessfulRequireCount,
                         qsizetype runCount,
                         qint64 elapsedTimeInNSecs);

private:
    QMap<Section const *, ScenarioPathRun> m_scenarioPaths;
};

}

#endif // SPECTATOR_SCENARIO_RUN_RESULTS_H
