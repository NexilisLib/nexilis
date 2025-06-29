#include <nexilis/object/object_3d.hh>

namespace nexilis
{

Object3D::Object3D(Object3D&& other) noexcept
    : Object(std::move(other))
{
}

Object3D& Object3D::operator=(Object3D&& other) noexcept
{
    if (this != &other)
    {
        Object::operator=(std::move(other));
    }
    return *this;
}

} // namespace nexilis
