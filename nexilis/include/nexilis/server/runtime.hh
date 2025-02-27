#ifndef NEXILIS_SERVER_RUNTIME_HH
#define NEXILIS_SERVER_RUNTIME_HH

#include <cstdint>
#include <thread>
#include <functional>

namespace nexilis::server
{

/// \tparam Condition The condition when to run update function.
/// \note Must have counter (size_t) as parameter.
/// \tparam Args The arguments for the update function.
/// \param condition The run condition.
/// \param f The update function.
/// \param sleep The sleep time for this function.
/// \param args Arguments for the update function.
/// \return False if the update function returns false.
template <typename Condition, typename ...Args>
bool runtime(Condition condition, const std::function<bool(Args...)>& f,
    uint32_t sleep, Args... args)
{
    for (std::size_t i = 0;;i++)
    {
        if (condition(i))
        {
            if (!f(std::forward<Args>(args)...))
            {
                return false;
            }
        }
        std::this_thread::sleep_for(std::chrono::seconds(sleep));
    }
    return true;
}

}

#endif