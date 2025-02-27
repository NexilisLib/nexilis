#ifndef NEXILIS_VECTOR3_HH
#define NEXILIS_VECTOR3_HH

#include <nexilis/nexilis_constants.hh>

#include <stdexcept>

namespace nexilis
{

template <typename T>
class Vector3
{
public:
    /// Constructor.
    /// \param x The x value of the vector3.
    /// \param y The y value of the vector3.
    /// \param z The z value of the vector3.
    Vector3(T x, T y, T z)
        : m_x(x), m_y(y), m_z(z)
    {
    }

    /// Default constructor.
    Vector3()
        : m_x(0), m_y(0), m_z(0)
    {
    }

    nx_data serialize() const
    {
        nx_data serializedData;

        // Convert each component into bytes.
        uint32_t xBytes = *reinterpret_cast<const uint32_t*>(&m_x);
        uint32_t yBytes = *reinterpret_cast<const uint32_t*>(&m_y);
        uint32_t zBytes = *reinterpret_cast<const uint32_t*>(&m_z);

        // Add component bytes to the serialized data.
        serializedData.insert(serializedData.end(), reinterpret_cast<const uint8_t*>(&xBytes),
                              reinterpret_cast<const uint8_t*>(&xBytes) + sizeof(uint32_t));
        serializedData.insert(serializedData.end(), reinterpret_cast<const uint8_t*>(&yBytes),
                              reinterpret_cast<const uint8_t*>(&yBytes) + sizeof(uint32_t));
        serializedData.insert(serializedData.end(), reinterpret_cast<const uint8_t*>(&zBytes),
                              reinterpret_cast<const uint8_t*>(&zBytes) + sizeof(uint32_t));

        return serializedData;
    }

    static Vector3 deserialize(const nx_data& data)
    {
        if (data.size() != sizeof(uint32_t) * 3)
        {
            // Handle invalid data size.
            throw std::runtime_error("Invalid data size for deserialization");
        }

        // Extract bytes and convert them back to float components.
        uint32_t xBytes = *reinterpret_cast<const uint32_t*>(&data[0]);
        uint32_t yBytes = *reinterpret_cast<const uint32_t*>(&data[sizeof(uint32_t)]);
        uint32_t zBytes = *reinterpret_cast<const uint32_t*>(&data[sizeof(uint32_t) * 2]);

        float x = *reinterpret_cast<const float*>(&xBytes);
        float y = *reinterpret_cast<const float*>(&yBytes);
        float z = *reinterpret_cast<const float*>(&zBytes);

        return Vector3(x, y, z);
    }

private:
    T m_x;
    T m_y;
    T m_z;
};

using Vector3f = Vector3<float>;
using Vector3u = Vector3<uint64_t>;
using Vector3i = Vector3<int>;

} // namespace nexilis

#endif
