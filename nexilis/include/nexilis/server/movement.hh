#ifndef NEXILIS_SERVER_MOVEMENT_HH
#define NEXILIS_SERVER_MOVEMENT_HH

#include <nexilis/movement/movement_2D.hh>
#include <nexilis/movement/movement_3D.hh>
#include <nexilis/server/settings.hh>
#include <nexilis/server/user.hh>

#include <thread>

namespace nexilis::server
{

class Movement
{
public:
    static bool isInitialized();
    static void _initialize(Settings& settings);

    /// Smooth movement.
    static double easing(double progress, double totalDistance);

    /// Linear movement.
    static double linear(double progress, double totalDistance);

    /// 2D movement thread.
    static std::thread object2D(std::unique_ptr<Movement2D> movement, User& user, Protocol& protocol);
    static std::thread object3D(std::unique_ptr<Movement3D> movement, User& user, Protocol& protocol);

private:
    static Settings* m_settings;
};

} // namespace nexilis::server
#endif
