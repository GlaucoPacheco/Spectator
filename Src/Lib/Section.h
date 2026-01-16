// Copyright (C) 2026 Glauco Pacheco <glaucopacheco@gmail.com>
// SPDX-License-Identifier: AGPL-3.0-only OR CDDL-1.0

#ifndef SPECTATOR_SECTION_H
#define SPECTATOR_SECTION_H

#include <QStringView>
#include <QtTypes>
#include <QtClassHelperMacros>

namespace Spectator
{

class Section
{
    Q_DISABLE_COPY_MOVE(Section)
public:
    Section(QStringView sourceFile, qint32 sourceLine, QStringView sectionName);
    ~Section() = default;
    inline QStringView sourceFile() const {return m_sourceFile;}
    inline qint32 sourceLine() const {return m_sourceLine;}
    inline QStringView name() const {return m_sectionName;}

private:
    const QStringView m_sourceFile;
    const qint32 m_sourceLine;
    const QStringView m_sectionName;
};

}

#endif // SPECTATOR_SECTION_H
