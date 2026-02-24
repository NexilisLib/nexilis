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
