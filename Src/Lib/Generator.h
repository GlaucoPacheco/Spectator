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

#include <QByteArrayView>
#include <QtClassHelperMacros>
#include <QtTypes>

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
    Generator(qsizetype size, QByteArrayView sourceFile, qint32 sourceLine);
    ~Generator() = default;
    template <typename T>
    T currentValue(std::initializer_list<T> initList) const
    {
        return initList.begin()[getGeneratorIndex(this)];
    }

    template <typename T>
    T currentRangeValue(T minVal, T maxVal, T stepVal) const
    {
        const qsizetype currentIndex = getGeneratorIndex(this);
        const auto currentValue = minVal + stepVal * currentIndex;
        return currentValue;
    }

private:
    static qsizetype getGeneratorIndex(Generator const * const generator);

private:
    const qsizetype m_size;
    const QByteArrayView m_sourceFile;
    const qint32 m_sourceLine;
};

}

#define AS(...) (::Spectator::GeneratorTypeHolder<__VA_ARGS__>)
#define GENERATE(TypeHolder, ...) \
    static constexpr std::initializer_list<TypeHolder::Type> _SPECTATOR_CONCATENATE_(_spectator_generator_list, __LINE__) = {__VA_ARGS__}; \
    static ::Spectator::Generator _SPECTATOR_CONCATENATE_(_spectator_generator, __LINE__)(_SPECTATOR_CONCATENATE_(_spectator_generator_list, __LINE__).size(), __FILE__, __LINE__); \
    _SPECTATOR_CONCATENATE_(_spectator_generator, __LINE__).currentValue<TypeHolder::Type>(_SPECTATOR_CONCATENATE_(_spectator_generator_list, __LINE__));
#define GENERATE_RANGE(TypeHolder, MIN_VAL, MAX_VAL) \
    static_assert(MAX_VAL > MIN_VAL); \
    static ::Spectator::Generator _SPECTATOR_CONCATENATE_(_spectator_generator, __LINE__)(MAX_VAL - MIN_VAL + 1, __FILE__, __LINE__); \
    _SPECTATOR_CONCATENATE_(_spectator_generator, __LINE__).currentRangeValue<TypeHolder::Type>(MIN_VAL, MAX_VAL, 1);
#define GENERATE_RANGE_WITH_STEP(TypeHolder, MIN_VAL, MAX_VAL, STEP_VAL) \
    static_assert(MAX_VAL > MIN_VAL && STEP_VAL > 0); \
    static ::Spectator::Generator _SPECTATOR_CONCATENATE_(_spectator_generator, __LINE__)((MAX_VAL - MIN_VAL + 1)/STEP_VAL, __FILE__, __LINE__); \
    _SPECTATOR_CONCATENATE_(_spectator_generator, __LINE__).currentRangeValue<TypeHolder::Type>(MIN_VAL, MAX_VAL, STEP_VAL);

#endif // SPECTATOR_GENERATOR_H
