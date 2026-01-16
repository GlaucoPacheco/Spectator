// Copyright (C) 2026 Glauco Pacheco <glaucopacheco@gmail.com>
// SPDX-License-Identifier: AGPL-3.0-only OR CDDL-1.0

#include "Section.h"

namespace Spectator
{

Section::Section(QStringView sourceFile, qint32 sourceLine, QStringView sectionName) :
    m_sourceFile(sourceFile),
    m_sourceLine(sourceLine),
    m_sectionName(sectionName)
{
}

}
