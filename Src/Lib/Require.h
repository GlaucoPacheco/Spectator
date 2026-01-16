// Copyright (C) 2026 Glauco Pacheco <glaucopacheco@gmail.com>
// SPDX-License-Identifier: AGPL-3.0-only OR CDDL-1.0

#ifndef SPECTATOR_REQUIRE_H
#define SPECTATOR_REQUIRE_H

#include "SpectatorGlobals.h"
#include "MacroHelpers.h"
#include <QStringView>
#include <QtTypes>

namespace Spectator
{

class SPECTATOR_LIB_EXPORT Require
{
public:
    static void require(bool expr, QStringView exprAsString, QStringView sourceFile, qint32 sourceLine);

private:
    Require() = delete;
    ~Require() = delete;
};

}

#define REQUIRE(...) ::Spectator::Require::require(__VA_ARGS__, _SPECTATOR_MAKE_UTF_16_STRING(__VA_ARGS__), _SPECTATOR_TO_UTF_16_STRING_LITERAL(__FILE__), __LINE__)

#endif // SPECTATOR_REQUIRE_H
