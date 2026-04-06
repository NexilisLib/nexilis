#ifndef NEXILIS_MOVEMENT_2DMOVEMENT_HH
#define NEXILIS_MOVEMENT_2DMOVEMENT_HH

#include <nexilis/movement/movement.hh>

#include <nexilis/types/vector2.hh>

namespace nexilis
{

class Movement2D : public Movement<Vector2f>
{
public:
    /// Constructor.
    explicit Movement2D(const MovementData& params, Vector2f movement_amount, const MovementFunc& movement_function)
        : Movement<Vector2f>(params),
          m_movementAmount(movement_amount),
          m_movementFunction(movement_function)
    {
    }

    /// Movement::getType implementation.
    Type getType() const override
    {
        return Type::_2D;
    }

    /// Movement::getAmount implementation.
    Vector2f getAmount() const override
    {
        return m_movementAmount;
    }

    MovementFunc getMovementFunc() const
    {
        return m_movementFunction;
    }

private:
    Vector2f m_movementAmount;
    MovementFunc m_movementFunction;
};

} // namespace nexilis

#endif
