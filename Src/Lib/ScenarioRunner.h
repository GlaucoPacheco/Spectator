// Copyright (C) 2026 Glauco Pacheco <glaucopacheco@gmail.com>
// SPDX-License-Identifier: AGPL-3.0-only OR CDDL-1.0

#ifndef SPECTATOR_SCENARIO_RUNNER_H
#define SPECTATOR_SCENARIO_RUNNER_H

#include "ScenarioRunResults.h"
#include <QStack>
#include <QSet>
#include <QMap>
#include <QString>
#include <QtClassHelperMacros>
#include <QtTypes>
#include <utility>

namespace Spectator
{

class Scenario;
class Section;
class Generator;

class ScenarioRunner
{
    Q_DISABLE_COPY_MOVE(ScenarioRunner)
public:
    ScenarioRunner(Scenario const * pScenario);
    ~ScenarioRunner() = default;
    bool tryPushSection(Section const * const pSection);
    void popSection(Section const * const pSection);
    qsizetype getGeneratorIndex(Generator const * const generator);
    static ScenarioRunner & current();
    static void incrementSuccessfulRequireCounter();
    static void incrementUnsuccessfulRequireCounter(QString failureMessage);
    static void recordInfoMessage(QString message);
    static ScenarioRunResults runScenario(const Scenario & scenario);
    static void printGlobalStats(QString &buffer);

private:
    inline static bool hasCurrent() {return m_pCurrentRunner != nullptr;}
    void runScenarioPath();
    void tryToAdvanceGeneratorsOnCurrentPath();
    inline bool hasConsumedAllGeneratorDataOnCurrentPath() const {return m_hasConsumedAllGeneratorDataOnCurrentPath;}
    inline bool hasVisitedAllLeafNodes() const {return m_hasVisitedAllLeafNodes && m_untouchedGenerators.isEmpty();}

private:
    static constinit thread_local ScenarioRunner * m_pCurrentRunner;
    Scenario const * const m_pScenario = nullptr;
    enum class State {HeadingForLeaf, HeadingForRoot};
    State m_state = State::HeadingForLeaf;
    ScenarioRunResults m_scenarioRunResults;
    QStack<Section const *> m_sectionsStack;
    QStack<Section const *> m_pathToLeafSection;
    QMap<Section const *, QSet<QString>> m_infoMessages;
    QStack<std::pair<Generator const *, qsizetype>> m_generatorsStack;
    qsizetype m_idxNextGenerator = 0;
    bool m_isValidatingGeneratorStack = false;
    QSet<Generator const *> m_touchedGenerators;
    QSet<Generator const *> m_untouchedGenerators;
    QSet<Section const *> m_sectionsWithFullyVisitedChildren;
    bool m_scenarioHasUnvisitedChildren = false;
    bool m_hasVisitedAllLeafNodes = false;
    qsizetype m_successfulRequireCounter = 0;
    qsizetype m_unsuccessfulRequireCounter = 0;
    bool m_hasConsumedAllGeneratorDataOnCurrentPath = false;
    QString m_failureMessage;
};

}

#endif // SPECTATOR_SCENARIO_RUNNER_H
