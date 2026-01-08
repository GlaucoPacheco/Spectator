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

#ifndef SPECTATOR_SPECTATOR_EXCEPTION_H
#define SPECTATOR_SPECTATOR_EXCEPTION_H

#include <QByteArray>
#include <QtTypes>
#include <exception>

namespace Spectator
{

class SpectatorException : public std::exception
{
public:
    SpectatorException(QByteArrayView message) : m_message(message) {}
    SpectatorException(QByteArrayView message, QByteArrayView sourceFile, qint32 sourceLine) :
        m_message(message)
    {
        m_message.append(" failed at file://")
                .append(sourceFile)
                .append(':')
                .append(QByteArray::number(sourceLine))
                .append('.');
    };
    ~SpectatorException() override = default;
    const char* what() const noexcept override {return m_message.constData();}

private:
    QByteArray m_message;
};

}

#endif // SPECTATOR_SPECTATOR_EXCEPTION_H
