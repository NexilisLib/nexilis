#ifndef NEXILIS_OBJECT2D_HH
#define NEXILIS_OBJECT2D_HH

#include <nexilis/vector2.hh>

namespace nexilis
{

class Object2D
{
public:
    /// Default constructor.
    Object2D() = default;

    /// Move constructor.
    Object2D(Object2D&& other);

    /// Move assignment operator.
    Object2D& operator=(Object2D&& other);

    /// Deleted copy constructor.
    Object2D(const Object2D& other) = delete;

    /// Deleted copy assignment operator.
    Object2D& operator=(const Object2D& other) = delete;

    void setPosition(float x, float y)
    {
        m_position = Vector2f(x, y);
    }

    Vector2f getPosition() const { return m_position; }

    void setDimensions(float width, float height)
    {
        m_dimensions = Vector2f(width, height);
    }

    Vector2f getDimensions() const { return m_dimensions; }

private:
    Vector2f m_position;
    Vector2f m_dimensions;
};

}

#endif
