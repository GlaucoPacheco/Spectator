// Copyright (C) 2026 Glauco Pacheco <glaucopacheco@gmail.com>
// SPDX-License-Identifier: AGPL-3.0-only OR MPL-2.0-no-copyleft-exception

#ifndef SPECTATOR_SCENARIO_H
#define SPECTATOR_SCENARIO_H

#include "Section.h"
#include "ScenarioRunner.h"
#include "SpectatorException.h"
#include <QStringView>
#include <QString>
#include <QtTypes>
#include <QtClassHelperMacros>
#include <source_location>

namespace Spectator
{

class Scenario : public Section
{
    Q_DISABLE_COPY_MOVE(Scenario)
public:
    Scenario(QStringView sourceFile, qint32 sourceLine, QStringView scenarioName);
    ~Scenario() = default;
    inline void REQUIRE(bool expr, const std::source_location location = std::source_location::current())
    {
        if (expr) [[likely]]
            m_scenarioRunner.incrementSuccessfulRequireCounter();
        else [[unlikely]]
            processFailedRequire(location);
    }
    inline void INFO(QString message) {m_scenarioRunner.recordInfoMessage(message);}
    inline void FAIL(QString message, const std::source_location location = std::source_location::current())
    {throw SpectatorException(message, QString::fromUtf8(location.file_name()), location.line());}

protected:
    virtual void ___scenarioFunction() = 0;

private:
    inline ScenarioRunner & scenarioRunner() {return m_scenarioRunner;}
    void processFailedRequire(const std::source_location location);

private:
    friend class Generator;
    friend class SectionGuard;
    friend class ScenarioRunner;
    ScenarioRunner m_scenarioRunner;
};

}

#define SCENARIO(SCENARIO_NAME) \
    class _SPECTATOR_CONCATENATE_(Scenario, __LINE__) : public ::Spectator::Scenario \
    { \
    public: \
        _SPECTATOR_CONCATENATE_(Scenario, __LINE__)(QStringView sourceFile, qint32 sourceLine, QStringView scenarioName) : ::Spectator::Scenario(sourceFile, sourceLine, scenarioName) {} \
        ~_SPECTATOR_CONCATENATE_(Scenario, __LINE__)() = default; \
    private: \
        void ___scenarioFunction() override; \
    }; \
    static _SPECTATOR_CONCATENATE_(Scenario, __LINE__) _SPECTATOR_CONCATENATE_(_spectator_scenario, __LINE__)(_SPECTATOR_TO_UTF_16_STRING_LITERAL(__FILE__), __LINE__, _SPECTATOR_TO_UTF_16_STRING_LITERAL("Scenario: " SCENARIO_NAME)); \
    void _SPECTATOR_CONCATENATE_(Scenario, __LINE__)::___scenarioFunction()

#endif // SPECTATOR_SCENARIO_H
