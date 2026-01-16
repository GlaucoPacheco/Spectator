// Copyright (C) 2026 Glauco Pacheco <glaucopacheco@gmail.com>
// SPDX-License-Identifier: AGPL-3.0-only OR CDDL-1.0

#include "SectionGuard.h"
#include "ScenarioRunner.h"

namespace Spectator
{

SectionGuard::SectionGuard(Section const * const section) :
    m_pSection(section),
    m_hasEntered(ScenarioRunner::current().tryPushSection(m_pSection))
{
    assert(m_pSection);
}

SectionGuard::~SectionGuard()
{
    if (m_hasEntered)
        ScenarioRunner::current().popSection(m_pSection);
}

}
