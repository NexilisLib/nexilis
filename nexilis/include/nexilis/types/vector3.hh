#ifndef NEXILIS_TYPES_VECTOR3_HH
#define NEXILIS_TYPES_VECTOR3_HH

#include <nexilis/nx_data.hh>
#include <nexilis/types/vector.hh>

#include <cstring>
#include <stdexcept>

namespace nexilis
{

template <typename T>
class Vector3 : public Vector
{
public:
    T x{0}, y{0}, z{0};

    /// Constructor.
    /// \param x The x value of the vector3.
    /// \param y The y value of the vector3.
    /// \param z The z value of the vector3.
    Vector3(T x, T y, T z)
        : x(x), y(y), z(z)
    {
    }

    /// Default constructor.
    Vector3() = default;

    /// Copy constructor.
    Vector3(const Vector3& other)
        : x(other.x),
          y(other.y),
          z(other.z)
    {
    }

    /// Copy assignment operator.
    Vector3& operator=(const Vector3& other)
    {
        if (this != &other)
        {
            x = other.x;
            y = other.y;
            z = other.z;
        }
        return *this;
    }

    /// Move constructor.
    Vector3(Vector3&& other) noexcept
        : x(std::move(other.x)),
          y(std::move(other.y)),
          z(std::move(other.z))
    {
    }

    /// Move assignment operator.
    Vector3& operator=(Vector3&& other) noexcept
    {
        if (this != &other)
        {
            x = std::move(other.x);
            y = std::move(other.y);
            z = std::move(other.z);
        }
        return *this;
    }

    /// Virtual destructor.
    virtual ~Vector3() = default;

    /// Vector::getType implementation.
    VectorType getType() override
    {
        return VectorType::vector3;
    }

    nx_data serialize() const
    {
        nx_data serializedData;

        // Convert the template argument into uint32.
        uint32_t xBytes, yBytes, zBytes;
        std::memcpy(&xBytes, &x, sizeof(xBytes));
        std::memcpy(&yBytes, &y, sizeof(yBytes));
        std::memcpy(&zBytes, &z, sizeof(zBytes));

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

        uint32_t xBytes, yBytes, zBytes;
        std::memcpy(&xBytes, data.data() + 0 * sizeof(uint32_t), sizeof(uint32_t));
        std::memcpy(&yBytes, data.data() + 1 * sizeof(uint32_t), sizeof(uint32_t));
        std::memcpy(&zBytes, data.data() + 2 * sizeof(uint32_t), sizeof(uint32_t));

        float _x, _y, _z;
        std::memcpy(&_x, &xBytes, sizeof(float));
        std::memcpy(&_y, &yBytes, sizeof(float));
        std::memcpy(&_z, &zBytes, sizeof(float));

        return Vector3(_x, _y, _z);
    }
};

using Vector3f = Vector3<float>;
using Vector3u = Vector3<uint64_t>;
using Vector3i = Vector3<int>;

} // namespace nexilis

#endif
