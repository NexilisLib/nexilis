#ifndef NEXILIS_OBJECT2D_HH
#define NEXILIS_OBJECT2D_HH

#include <nexilis/object/object.hh>
#include <nexilis/types/vector2.hh>

namespace nexilis
{

class Object2D : public Object<Vector2f>
{
public:
    /// Constructor.
    explicit Object2D(uint64_t id, const Vector2f& position = Vector2f(), const Vector2f& dimensions = Vector2f(1.f, 1.f))
        : Object(id, position, dimensions)
    {
    }

    /// Deleted copy constructor.
    Object2D(const Object&) = delete;

    /// Deleted copy assignment operator.
    Object2D& operator=(const Object2D&) = delete;

    /// Move constructor.
    Object2D(Object2D&& other) noexcept;

    /// Move assignment operator.
    Object2D& operator=(Object2D&& other) noexcept;

    nx_data getData() override
    {
        return baseData();
    }
};

} // namespace nexilis

#endif
