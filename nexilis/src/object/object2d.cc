#include <nexilis/object/object2d.hh>

namespace nexilis
{

Object2D::Object2D(Object2D&& other)
    : m_position(std::move(other.m_position)),
      m_dimensions(std::move(other.m_dimensions))
{
}

Object2D& Object2D::operator=(Object2D&& other)
{
    if (this != &other)
    {
        m_position = std::move(other.m_position);
        m_dimensions = std::move(other.m_dimensions);
    }
    return *this;
}

} // namespace nexilis
