/* Copyright (C) 2026 Valtteri Viirret
   This file is part of the Nexilis Project.

   This file is free software: you can redistribute it and/or modify
   it under the terms of the GNU Lesser General Public License as
   published by the Free Software Foundation, either version 3 of the
   License, or (at your option) any later version.

   This file is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public License
   along with this file.  If not, see <https://gnu.org>. */

#ifndef NEXILIS_SERVER_RUNTIME_HH
#define NEXILIS_SERVER_RUNTIME_HH

#include <chrono>
#include <functional>
#include <mutex>
#include <thread>

namespace nexilis::server
{

/// Control server runtime.
/// \tparam Condition The condition when to run update function.
/// \note The condition must have "counter" (size_t) as a parameter.
/// \tparam Args The arguments for the update function.
/// \param condition When to run the update function.
/// The update condition logic can be formed from the "counter" parameter.
/// \param f The update function.
/// \param tickrate The tickrate for the update function.
/// \param args Arguments for the update function.
/// \return False if the update function returns false.
template <typename Condition, typename... Args>
bool runtime(const Condition& condition, const std::function<bool(Args...)>& f,
             uint32_t tickrate, Args... args)
{
    using clock = std::chrono::steady_clock;
    auto tick_duration = std::chrono::milliseconds(1000 / tickrate);
    std::mutex mtx;

    for (size_t i = 0;; i++)
    {
        auto start_time = clock::now();

        {
            std::lock_guard<std::mutex> lock(mtx);
            if (condition(i))
            {
                if (!f(std::forward<Args>(args)...))
                {
                    return false;
                }
            }
        }
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(clock::now() - start_time);
        if (elapsed < tick_duration)
        {
            std::this_thread::sleep_for(tick_duration - elapsed);
        }
    }
    return true;
}

} // namespace nexilis::server

#endif
