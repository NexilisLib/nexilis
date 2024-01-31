#ifndef NEXILIS_VECTOR3_HH
#define NEXILIS_VECTOR3_HH

#include <cstdint>
#include <vector>

/// Vector3 object to be used in update messages

/// Create or receive one with 3 floats (12 bytes),
/// or use with messages std::vector<uint8_t> (4 bytes).

class Vector3
{
public:
    /// Constructor.
    /// \param x The x value of the vector3.
    /// \param y The y value of the vector3.
    /// \param z The z value of the vector3.
    explicit Vector3(float x, float y, float z);

    std::vector<uint8_t> serialize() const;
    //static Vector3 deserialize(const std::vector<uint8_t>& data);

private:
    float m_x;
    float m_y;
    float m_z;
};

#endif
