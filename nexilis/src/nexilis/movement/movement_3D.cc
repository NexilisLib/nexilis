#include <nexilis/movement/movement_3D.hh>

namespace nexilis
{

Movement3D::Movement3D(const MovementData& params, Vector3f movement_amount, const MovementFunc& movement_function)
    : Movement<Vector3f>(params),
      m_movementAmount(movement_amount),
      m_movementFunction(movement_function)
{
}

} // namespace nexilis
