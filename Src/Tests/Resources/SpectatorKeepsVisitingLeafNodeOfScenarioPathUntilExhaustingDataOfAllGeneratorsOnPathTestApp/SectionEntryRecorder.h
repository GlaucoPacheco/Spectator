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

#ifndef SPECTATOR_SECTION_ENTRY_RECORDER_H
#define SPECTATOR_SECTION_ENTRY_RECORDER_H

#include <QMap>
#include <QString>
#include <QtTypes>
#include <QtClassHelperMacros>
#include <QByteArray>
#include <QList>
#include <cstdint>

struct Data
{
    QString string;
    int intValue = 0;
    int64_t int64Value = 0;
    QByteArray byteArray;
};

namespace Spectator::Test
{

class SectionEntryRecorder
{
    Q_DISABLE_COPY_MOVE(SectionEntryRecorder)
public:
    ~SectionEntryRecorder() = default;
    static SectionEntryRecorder & global();
    inline void recordEntry(QString sectionName) {++m_entryRecords[sectionName];}
    inline void recordData(const Data & data) {m_recordedData.append(data);}
    inline QMap<QString, qint64> recordedEntries() const {return m_entryRecords;}
    inline QList<Data> recordedData() const {return m_recordedData;}

private:
    SectionEntryRecorder() = default;

private:
    QMap<QString, qint64> m_entryRecords;
    QList<Data> m_recordedData;
};

}

#endif // SPECTATOR_SECTION_ENTRY_RECORDER_H
