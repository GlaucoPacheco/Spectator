// Copyright (C) 2026 Glauco Pacheco <glaucopacheco@gmail.com>
// SPDX-License-Identifier: AGPL-3.0-only OR MPL-2.0-no-copyleft-exception

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
    ScenarioRunResults(const ScenarioRunResults &) = default;
    ~ScenarioRunResults() = default;
    void addScenarioPath(QStack<Section const *> pathToLeafSection,
                         QMap<Section const *, QSet<QString>> infoMessages,
                         QString fatalMessage,
                         qsizetype successfulRequireCount,
                         qsizetype unsuccessfulRequireCount,
                         qsizetype runCount,
                         qint64 elapsedTimeInNSecs);
    inline QMap<Section const *, ScenarioPathRun> scenarioPaths() const {return m_scenarioPaths;}

private:
    QMap<Section const *, ScenarioPathRun> m_scenarioPaths;
};

}

#endif // SPECTATOR_SCENARIO_RUN_RESULTS_H
