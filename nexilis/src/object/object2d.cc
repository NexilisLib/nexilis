#include <nexilis/object/object2d.hh>

namespace nexilis
{

Object2D::Object2D(Object2D&& other)
    : m_position(std::move(other.m_position)),
      m_dimensions(std::move(other.m_dimensions)),
      m_filepath(std::move(other.m_filepath))
{
}

Object2D& Object2D::operator=(Object2D&& other)
{
    if (this != &other)
    {
        m_position = std::move(other.m_position);
        m_dimensions = std::move(other.m_dimensions);
        m_filepath = std::move(other.m_filepath);
    }
    return *this;
}

} // namespace nexilis
