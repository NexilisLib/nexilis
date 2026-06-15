#ifndef NEXILIS_ROOM_INFO_HH
#define NEXILIS_ROOM_INFO_HH

#include <cstdint>
#include <string>

namespace nexilis
{

class RoomInfo
{
public:
    // Constructor
    RoomInfo(uint64_t id, const std::string& name)
        : m_id(id), m_name(name)
    {
    }

    uint64_t getId() const
    {
        return m_id;
    }
    const std::string& getName() const
    {
        return m_name;
    }

private:
    uint64_t m_id;
    std::string m_name;
};

} // namespace nexilis

#endif
