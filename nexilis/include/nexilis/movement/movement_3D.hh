#ifndef NEXILIS_MOVEMENT_MOVEMENT_3D_HH
#define NEXILIS_MOVEMENT_MOVEMENT_3D_HH

#include <nexilis/movement/movement.hh>
#include <nexilis/types/vector3.hh>

namespace nexilis
{

class Movement3D : public Movement<Vector3f>
{
public:
    /// Constructor.
    explicit Movement3D(const MovementData& params, Vector3f movement_amount, const MovementFunc& movement_function);

    /// Movement::getType implementation.
    Type getType() const override
    {
        return Type::_3D;
    }

    /// Movement::getAmount implementation.
    Vector3f getAmount() const override
    {
        return m_movementAmount;
    }

    MovementFunc getMovementFunc() const
    {
        return m_movementFunction;
    }

private:
    Vector3f m_movementAmount;
    MovementFunc m_movementFunction;
};

} // namespace nexilis

#endif
