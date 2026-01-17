// Copyright (C) 2026 Glauco Pacheco <glaucopacheco@gmail.com>
// SPDX-License-Identifier: AGPL-3.0-only OR MPL-2.0-no-copyleft-exception

#ifndef SPECTATOR_SCENARIO_H
#define SPECTATOR_SCENARIO_H

#include "MacroHelpers.h"
#include "Section.h"
#include "ScenarioRunner.h"
#include "SpectatorException.h"
#include <QStringView>
#include <QString>
#include <QtTypes>
#include <QtClassHelperMacros>
#include <array>
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
            m_pScenarioRunner->incrementSuccessfulRequireCounter();
        else [[unlikely]]
            processFailedRequire(location);
    }
    inline void INFO(QString message) {m_pScenarioRunner->recordInfoMessage(message);}
    inline void FAIL(QString message, const std::source_location location = std::source_location::current())
    {throw SpectatorException(message, QString::fromUtf8(location.file_name()), location.line());}
    virtual qsizetype tagCount() const = 0;
    virtual QStringView tagAt(qsizetype idx) const = 0;

protected:
    virtual void ___scenarioFunction() = 0;

private:
    inline ScenarioRunner * scenarioRunner() {return m_pScenarioRunner;}
    void processFailedRequire(const std::source_location location);

private:
    friend class Generator;
    friend class SectionGuard;
    friend class ScenarioRunner;
    ScenarioRunner * m_pScenarioRunner;
};

}

#define TAG(t) _SPECTATOR_TO_UTF_16_STRING_LITERAL(t)
#define SCENARIO(SCENARIO_NAME, ...) \
    class _SPECTATOR_CONCATENATE_(Scenario, __LINE__) : public ::Spectator::Scenario \
    { \
    public: \
        _SPECTATOR_CONCATENATE_(Scenario, __LINE__)(QStringView sourceFile, qint32 sourceLine, QStringView scenarioName) : ::Spectator::Scenario(sourceFile, sourceLine, scenarioName) {} \
        ~_SPECTATOR_CONCATENATE_(Scenario, __LINE__)() = default; \
        qsizetype tagCount() const override {return m_tags.size() - 1;} \
        QStringView tagAt(qsizetype idx) const override {return m_tags[idx + 1];} \
    private: \
        void ___scenarioFunction() override; \
        static constexpr auto m_tags = std::to_array({u"" __VA_OPT__(,) __VA_ARGS__}); \
    }; \
    static _SPECTATOR_CONCATENATE_(Scenario, __LINE__) _SPECTATOR_CONCATENATE_(_spectator_scenario, __LINE__)(_SPECTATOR_TO_UTF_16_STRING_LITERAL(__FILE__), __LINE__, _SPECTATOR_TO_UTF_16_STRING_LITERAL("Scenario: " SCENARIO_NAME)); \
    void _SPECTATOR_CONCATENATE_(Scenario, __LINE__)::___scenarioFunction()

#endif // SPECTATOR_SCENARIO_H
