#include <nexilis/movement/movement_2D.hh>

namespace nexilis
{

Movement2D::Movement2D(const Movement::Data& data, Vector2f movement_amount, const MovementFunc& movement_function)
    : Movement(data),
      m_movementAmount(movement_amount),
      m_movementFunction(movement_function)
{
}

}