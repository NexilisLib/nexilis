#ifndef NEXILIS_OBJECT3D_HH
#define NEXILIS_OBJECT3D_HH

#include <nexilis/object/object.hh>
#include <nexilis/types/vector3.hh>

namespace nexilis
{

class Object3D : public Object<Vector3f>
{
public:
    /// Constructor.
    explicit Object3D(uint64_t id, const Vector3f& position = Vector3f(), const Vector3f& dimensions = Vector3f(1.f, 1.f, 1.f))
        : Object(id, position, dimensions)
    {
    }

    /// Deleted copy constructor.
    Object3D(const Object3D& other) = delete;

    /// Deleted copy assignment operator.
    Object3D& operator=(const Object3D& other) = delete;

    /// Move constructor.
    Object3D(Object3D&& other) noexcept;

    /// Move assignment operator.
    Object3D& operator=(Object3D&& other) noexcept;

    nx_data getData() override
    {
        nx_data a;
        return a;
    }
};

} // namespace nexilis

#endif
