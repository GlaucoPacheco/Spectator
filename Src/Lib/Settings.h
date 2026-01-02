// SPDX-License-Identifier: CDDL-1.0
//
// CDDL HEADER START
//
// The contents of this file are subject to the terms of the
// Common Development and Distribution License 1.0 (the "License").
// You may not use this file except in compliance with the License.
//
// You can obtain a copy of the license at usr/src/OPENSOLARIS.LICENSE
// or https://opensource.org/licenses/CDDL-1.0.
// See the License for the specific language governing permissions
// and limitations under the License.
//
// When distributing Covered Code, include this CDDL HEADER in each
// file and include the License file at usr/src/OPENSOLARIS.LICENSE.
// If applicable, add the following below this CDDL HEADER, with the
// fields enclosed by brackets "[]" replaced with your own identifying
// information: Portions Copyright [yyyy] [name of copyright owner]
//
// CDDL HEADER END
//

//
// Copyright (C) 2025 Glauco Pacheco <glaucopacheco@gmail.com>
//

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
    int threadCount() const {return m_threadCount;}
    quint64 repetitionCount() const {return m_repetitionCount;}
    QString filePathFilter() const {return m_filePathFilter;}
    QString scenarioNameFilter() const {return m_scenarioNameFilter;}
    QStringList scenarioTagsFilter() const {return m_scenarioTagsFilter;}
    static Settings fromCmdLine();

private:
    int m_threadCount = 1;
    quint64 m_repetitionCount = 1;
    QString m_filePathFilter;
    QString m_scenarioNameFilter;
    QStringList m_scenarioTagsFilter;
};

}

#endif // SPECTATOR_SETTINGS_H
