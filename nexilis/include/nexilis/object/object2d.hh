#ifndef NEXILIS_OBJECT2D_HH
#define NEXILIS_OBJECT2D_HH

#include <nexilis/object/object.hh>
#include <nexilis/types/vector2.hh>

namespace nexilis
{

class Object2D : public Object<Vector2f>
{
public:
    explicit Object2D(uint64_t id, const Vector2f& position = {0.f, 0.f}, const Vector2f& dimensions = {0.f, 0.f})
        : Object(id, position, dimensions)
    {
    }

    nx_data getData() override
    {
        return baseData();
    }
};

} // namespace nexilis

#endif
