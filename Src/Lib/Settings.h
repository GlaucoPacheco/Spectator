// Copyright (C) 2026 Glauco Pacheco <glaucopacheco@gmail.com>
// SPDX-License-Identifier: AGPL-3.0-only OR CDDL-1.0

#ifndef SPECTATOR_SETTINGS_H
#define SPECTATOR_SETTINGS_H

#include <QString>
#include <QStringList>
#include <QtTypes>

namespace Spectator
{

class Settings
{
public:
    Settings() = default;
    ~Settings() = default;
    qint32 threadCount() const {return m_threadCount;}
    qint64 repetitionCount() const {return m_repetitionCount;}
    QString filePathFilter() const {return m_filePathFilter;}
    QString scenarioNameFilter() const {return m_scenarioNameFilter;}
    QStringList scenarioTagsFilter() const {return m_scenarioTagsFilter;}
    static Settings fromCmdLine();

private:
    qint32 m_threadCount = 1;
    qint64 m_repetitionCount = 0;
    QString m_filePathFilter;
    QString m_scenarioNameFilter;
    QStringList m_scenarioTagsFilter;
};

}

#endif // SPECTATOR_SETTINGS_H
