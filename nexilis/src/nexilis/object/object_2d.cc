#include <nexilis/object/object_2d.hh>

namespace nexilis
{

Object2D::Object2D(Object2D&& other) noexcept
    : Object(std::move(other))
{
}

Object2D& Object2D::operator=(Object2D&& other) noexcept
{
    if (this != &other)
    {
        Object::operator=(std::move(other));
    }
    return *this;
}

} // namespace nexilis
