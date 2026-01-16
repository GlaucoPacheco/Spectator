// Copyright (C) 2026 Glauco Pacheco <glaucopacheco@gmail.com>
// SPDX-License-Identifier: AGPL-3.0-only OR CDDL-1.0

#ifndef SPECTATOR_SPECTATOR_GLOBALS_H
#define SPECTATOR_SPECTATOR_GLOBALS_H

#include <QtCore/QtGlobal>

#if defined(SPECTATOR_LIB_LIBRARY)
    #define SPECTATOR_LIB_EXPORT Q_DECL_EXPORT
 #else
    #define SPECTATOR_LIB_EXPORT Q_DECL_IMPORT
 #endif

namespace Spectator
{



}

#endif // SPECTATOR_SPECTATOR_GLOBALS_H
