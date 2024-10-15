#ifndef NEXILIS_OBJECT3D_HH
#define NEXILIS_OBJECT3D_HH

#include <nexilis/object/object.hh>
#include <nexilis/types/vector3.hh>

namespace nexilis
{

class Object3D : public Object<Vector3>
{
public:
    Object3D(const Vector3& position, const Vector3& dimensions) 
        : Object(position, dimensions)
    {
    }
};

} // namespace nexilis

#endif
