// Copyright (C) 2026 Glauco Pacheco <glaucopacheco@gmail.com>
// SPDX-License-Identifier: AGPL-3.0-only OR CDDL-1.0

#include "Generator.h"
#include "ScenarioRunner.h"

namespace Spectator
{

Generator::Generator(qsizetype size, QStringView sourceFile, qint32 sourceLine) :
    m_size(size),
    m_sourceFile(sourceFile),
    m_sourceLine(sourceLine)
{
}

qsizetype Generator::getGeneratorIndex(Generator const * const generator)
{
    return ScenarioRunner::current().getGeneratorIndex(generator);
}

}
