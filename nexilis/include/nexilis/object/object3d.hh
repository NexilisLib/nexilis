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
    Object3D(uint64_t id, const Vector3f& position, const Vector3f& dimensions)
        : Object(id, position, dimensions)
    {
    }

    nx_data getData() override
    {
        nx_data a;
        return a;
    }
};

} // namespace nexilis

#endif
