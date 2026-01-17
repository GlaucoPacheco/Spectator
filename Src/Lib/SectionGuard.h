// Copyright (C) 2026 Glauco Pacheco <glaucopacheco@gmail.com>
// SPDX-License-Identifier: AGPL-3.0-only OR MPL-2.0-no-copyleft-exception

#ifndef SPECTATOR_SECTION_GUARD_H
#define SPECTATOR_SECTION_GUARD_H

#include "Section.h"
#include "Scenario.h"
#include "ScenarioRunner.h"
#include <QtClassHelperMacros>

namespace Spectator
{

class Section;

class SectionGuard
{
    Q_DISABLE_COPY_MOVE(SectionGuard)
public:
    SectionGuard(Section const * const section, Scenario * pScenario) :
        m_pSection(section),
        m_pScenario(pScenario),
        m_hasEntered(m_pScenario->scenarioRunner()->tryPushSection(m_pSection))
    {
        assert(m_pSection);
    }
    ~SectionGuard()
    {
        if (m_hasEntered)
            m_pScenario->scenarioRunner()->popSection(m_pSection);
    }
    inline bool hasEntered() const {return m_hasEntered;}

private:
    Section const * const m_pSection;
    Scenario * m_pScenario = nullptr;
    const bool m_hasEntered;
};

}

#endif // SPECTATOR_SECTION_GUARD_H
