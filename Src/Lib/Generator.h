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

#ifndef SPECTATOR_GENERATOR_H
#define SPECTATOR_GENERATOR_H

#include <QStringView>
#include <QtClassHelperMacros>
#include <QtTypes>
#include <limits>

namespace Spectator
{

template <class T>
struct GeneratorTypeHolder
{
    using Type = T;
};

class Generator
{
    Q_DISABLE_COPY_MOVE(Generator)
public:
    Generator(qsizetype size, QStringView sourceFile, qint32 sourceLine);
    ~Generator() = default;
    template <class T>
    const T & currentValue(T const * const pData) const
    {
        return pData[getGeneratorIndex(this)];
    }

    template <class T>
    T currentRangeValue(T minVal, T maxVal, T stepVal) const
    {
        const qsizetype currentIndex = getGeneratorIndex(this);
        const auto currentValue = minVal + stepVal * currentIndex;
        return currentValue;
    }
    inline qsizetype size() const {return m_size;}
    inline QStringView sourceFile() const {return m_sourceFile;}
    inline qint32 sourceLine() const {return m_sourceLine;}

private:
    static qsizetype getGeneratorIndex(Generator const * const generator);

private:
    const qsizetype m_size;
    const QStringView m_sourceFile;
    const qint32 m_sourceLine;
};

}

#define AS(...) (::Spectator::GeneratorTypeHolder<__VA_ARGS__>)
#define GET_TYPE(...) __VA_ARGS__::Type
#define GENERATE(TypeHolder, ...) \
    []() -> const GET_TYPE TypeHolder & { \
        const static GET_TYPE TypeHolder data[] = {__VA_ARGS__}; \
        static_assert(sizeof(data)/sizeof(GET_TYPE TypeHolder) > 0); \
        const static ::Spectator::Generator generator(sizeof(data)/sizeof(GET_TYPE TypeHolder), _SPECTATOR_TO_UTF_16_STRING_LITERAL(__FILE__), __LINE__); \
        return generator.currentValue<GET_TYPE TypeHolder>(data); \
    }()
#define GENERATE_RANGE(TypeHolder, MIN_VAL, MAX_VAL) \
    []() -> GET_TYPE TypeHolder { \
        static_assert(std::numeric_limits<GET_TYPE TypeHolder>::is_integer && MAX_VAL > MIN_VAL); \
        static ::Spectator::Generator generator(MAX_VAL - MIN_VAL + 1, _SPECTATOR_TO_UTF_16_STRING_LITERAL(__FILE__), __LINE__); \
        return generator.currentRangeValue<GET_TYPE TypeHolder>(MIN_VAL, MAX_VAL, 1); \
    }()
#define GENERATE_RANGE_WITH_STEP(TypeHolder, MIN_VAL, MAX_VAL, STEP_VAL) \
    []() -> GET_TYPE TypeHolder { \
        static_assert(std::numeric_limits<GET_TYPE TypeHolder>::is_integer && MAX_VAL > MIN_VAL && STEP_VAL > 0); \
        static ::Spectator::Generator generator((MAX_VAL - MIN_VAL + 1)/STEP_VAL, _SPECTATOR_TO_UTF_16_STRING_LITERAL(__FILE__), __LINE__); \
        return generator.currentRangeValue<GET_TYPE TypeHolder>(MIN_VAL, MAX_VAL, STEP_VAL); \
    }()

#endif // SPECTATOR_GENERATOR_H
