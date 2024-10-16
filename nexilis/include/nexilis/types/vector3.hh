#ifndef NEXILIS_VECTOR3_HH
#define NEXILIS_VECTOR3_HH

#include <nexilis/nexilis_macros.hh>

/// Vector3 object to be used in update messages

/// Create or receive one with 3 floats (12 bytes),
/// or use with messages nx_data (4 bytes).

namespace nexilis
{

class Vector3
{
public:
    /// Constructor.
    /// \param x The x value of the vector3.
    /// \param y The y value of the vector3.
    /// \param z The z value of the vector3.
    explicit Vector3(float x, float y, float z);

    Vector3()
        : m_x(0), m_y(0), m_z(0)
    {
    }

    nx_data serialize() const;
    static Vector3 deserialize(const nx_data& data);

private:
    float m_x;
    float m_y;
    float m_z;
};

} // namespace nexilis

#endif
