#ifndef NEXILIS_OBJECT2D_HH
#define NEXILIS_OBJECT2D_HH

#include <nexilis/types/vector2.hh>
#include <nexilis/object/object.hh>

namespace nexilis
{

class Object2D : public Object<Vector2f>
{
public:
    Object2D(const Vector2f& position = {0.f, 0.f}, const Vector2f& dimensions = {0.f, 0.f}) 
        : Object(position, dimensions)
    {
    }
};

} // namespace nexilis

#endif
