// Copyright (C) 2026 Glauco Pacheco <glaucopacheco@gmail.com>
// SPDX-License-Identifier: AGPL-3.0-only OR CDDL-1.0

#ifndef SPECTATOR_SCENARIO_H
#define SPECTATOR_SCENARIO_H

#include "Section.h"
#include "MacroHelpers.h"
#include <QStringView>
#include <QtTypes>
#include <QtClassHelperMacros>
#include <functional>

namespace Spectator
{

class Scenario : public Section
{
    Q_DISABLE_COPY_MOVE(Scenario)
public:
    using F = void(*)();
    Scenario(QStringView sourceFile, qint32 sourceLine, QStringView scenarioName, F f);
    ~Scenario() = default;
    inline std::function<void()> scenarioFunction() const {return m_scenarioFunction;}

private:
    const std::function<void()> m_scenarioFunction;
};

}

#define SCENARIO(SCENARIO_NAME) static void _SPECTATOR_CONCATENATE_(_spectator_scenario_fcn_, __LINE__)(); \
static ::Spectator::Scenario _SPECTATOR_CONCATENATE_(_spectator_scenario, __LINE__)(_SPECTATOR_TO_UTF_16_STRING_LITERAL(__FILE__), __LINE__, _SPECTATOR_TO_UTF_16_STRING_LITERAL("Scenario: " SCENARIO_NAME), _SPECTATOR_CONCATENATE_(_spectator_scenario_fcn_, __LINE__)); \
static void _SPECTATOR_CONCATENATE_(_spectator_scenario_fcn_, __LINE__)()

#endif // SPECTATOR_SCENARIO_H
