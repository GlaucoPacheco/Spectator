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

#ifndef SPECTATOR_SECTION_GUARD_H
#define SPECTATOR_SECTION_GUARD_H

#include <QtClassHelperMacros>

namespace Spectator
{

class Section;

class SectionGuard
{
    Q_DISABLE_COPY_MOVE(SectionGuard)
public:
    SectionGuard(Section const * const section);
    ~SectionGuard();
    inline bool hasEntered() const {return m_hasEntered;}

private:
    Section const * const m_pSection;
    const bool m_hasEntered;
};

}

#endif // SPECTATOR_SECTION_GUARD_H
