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

#ifndef SPECTATOR_NO_DESTROY_H
#define SPECTATOR_NO_DESTROY_H

#include <utility>
#include <type_traits>
#include <functional>

namespace Spectator
{

template <class T>
class NoDestroy
{
public:
    template <class... Args>
    constexpr NoDestroy(Args... args) : m_object(std::forward<Args...>(args)...) {}
    ~NoDestroy() {}
    inline T &operator()() {return m_object;}
    inline const T &operator()() const {return m_object;}

private:
    union {T m_object;};
};

template <class T>
class NoDestroyPtrDeleter
{
    static_assert(std::is_pointer_v<T>);
    using T_CleanupFcn = void(T);
public:
    NoDestroyPtrDeleter(NoDestroy<T> &var) : m_var(var) {}
    NoDestroyPtrDeleter(NoDestroy<T> &var, T_CleanupFcn cleanupFcn) :
        m_var(var),
        m_cleanupFcn(cleanupFcn)
    {
    }
    ~NoDestroyPtrDeleter()
    {
        if (m_var())
        {
            if (m_cleanupFcn)
                m_cleanupFcn(m_var());
            delete m_var();
            m_var() = nullptr;
        }
    }

private:
    NoDestroy<T> &m_var;
    std::function<void(T)> m_cleanupFcn;
};

template <class T>
class NoDestroyCleaner
{
    static_assert(!std::is_pointer_v<T>);
    typedef void (T::*T_Cleaner)();
public:
    NoDestroyCleaner(NoDestroy<T> &var, T_Cleaner pCleanerFcn) :
        m_var(var),
        m_pCleanerFcn(pCleanerFcn)
    {
        assert(m_pCleanerFcn != nullptr);
    }
    ~NoDestroyCleaner() {(m_var().*m_pCleanerFcn)();}

private:
    NoDestroy<T> &m_var;
    T_Cleaner m_pCleanerFcn;
};

}

#endif // SPECTATOR_NO_DESTROY_H
