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

#include "ScenarioRunResults.h"

namespace Spectator
{

void ScenarioRunResults::addScenarioPath(QStack<Section const *> pathToLeafSection,
                                         qsizetype successfullRequireCount,
                                         qsizetype runCount,
                                         qint64 elapsedTimeInNSecs)
{
    if (!m_scenarioPaths.contains(pathToLeafSection.top()))
        m_scenarioPaths.insert(pathToLeafSection.top(), {.pathToLeafSection = pathToLeafSection,
                                .successfullRequireCount = successfullRequireCount,
                                .runCount = runCount,
                                .elapsedTimeInNSecs = elapsedTimeInNSecs});
    else
    {
        auto & scenarioPathRun = m_scenarioPaths[pathToLeafSection.top()];
        scenarioPathRun.successfullRequireCount += successfullRequireCount;
        scenarioPathRun.runCount += runCount;
        scenarioPathRun.elapsedTimeInNSecs += elapsedTimeInNSecs;
    }
}

}
