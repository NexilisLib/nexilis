#ifndef NEXILIS_MOVEMENT_2DMOVEMENT_HH
#define NEXILIS_MOVEMENT_2DMOVEMENT_HH

#include <nexilis/movement/movement.hh>

#include <nexilis/types/vector2.hh>

#include <functional>

namespace nexilis
{

class Movement2D : public Movement
{
public:
    using MovementFunc = std::function<double(double, double)>;

    /// Constructor.
    explicit Movement2D(const Movement::Data& params, Vector2f movement_amount, const MovementFunc& movement_function);

    Vector2f getMovementAmount() const
    {
        return m_movementAmount;
    }

    MovementFunc getMovementFunc() const
    {
        return m_movementFunction;
    }

private:
    Vector2f m_movementAmount;
    std::function<double(double, double)> m_movementFunction;
};

}

#endif
