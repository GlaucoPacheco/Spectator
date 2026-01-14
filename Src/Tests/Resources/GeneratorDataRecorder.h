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

#ifndef SPECTATOR_GENERATOR_DATA_RECORDER_H
#define SPECTATOR_GENERATOR_DATA_RECORDER_H

#include <QString>
#include <QtTypes>
#include <QtClassHelperMacros>
#include <QByteArray>
#include <QDataStream>
#include <QList>
#include <cstdint>

namespace Spectator::Test
{

struct GeneratorData
{
    QString string;
    int intValue = 0;
    int64_t int64Value = 0;
    QByteArray byteArray;

    friend inline QDataStream &operator<<(QDataStream &stream, const GeneratorData &data)
    {
        stream << data.string;
        stream << data.intValue;
        stream << qint64(data.int64Value);
        stream << data.byteArray;
        return stream;
    }

    friend inline QDataStream &operator>>(QDataStream &stream, GeneratorData & data)
    {
        stream >> data.string;
        stream >> data.intValue;
        qint64 val = 0;
        stream >> val;
        data.int64Value = val;
        stream >> data.byteArray;
        return stream;
    }
};

class GeneratorDataRecorder
{
    Q_DISABLE_COPY_MOVE(GeneratorDataRecorder)
public:
    ~GeneratorDataRecorder() = default;
    static GeneratorDataRecorder & global();
    inline void recordData(const GeneratorData & data) {m_recordedData.append(data);}
    inline QList<GeneratorData> recordedData() const {return m_recordedData;}

private:
    GeneratorDataRecorder() = default;

private:
    QList<GeneratorData> m_recordedData;
};

}

#endif // SPECTATOR_GENERATOR_DATA_RECORDER_H
