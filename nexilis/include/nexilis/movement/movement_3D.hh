#ifndef NEXILIS_MOVEMENT_MOVEMENT_3D_HH
#define NEXILIS_MOVEMENT_MOVEMENT_3D_HH

#include <nexilis/movement/movement.hh>
#include <nexilis/types/vector3.hh>

#include <functional>

namespace nexilis
{

class Movement3D : public Movement
{
public:
    using MovementFunc = std::function<double(double, double, double)>;
    /// Constructor.
    explicit Movement3D(const Movement::Data& params, Vector3f movement_amount, const MovementFunc& movement_function);

    Vector3f getMovementAmount() const
    {
        return m_movementAmount;
    }

    MovementFunc getMovement() const
    {
        return m_movementFunction;
    }

private:
    Vector3f m_movementAmount;
    std::function<double(double, double, double)> m_movementFunction;
};

} // namespace nexilis

#endif
