
// TODO correct the include paths
#include "../include/nexilis/vector3.hh"

#include <stdexcept>

Vector3::Vector3(float x, float y, float z) : 
    m_x(x), m_y(y), m_z(z)
{
}


std::vector<uint8_t> Vector3::serialize() const
{
    std::vector<uint8_t> serializedData;

    // Convert each component into bytes.
    int32_t xBytes = *reinterpret_cast<const uint32_t*>(&m_x);
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


static Vector3 deserialize(const std::vector<uint8_t>& data) 
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

