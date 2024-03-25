#include <nexilis/common/util.hh>
#include <nexilis/room.hh>

namespace nexilis
{

Room::Settings::Settings(const std::string& name, uint32_t maxSize)
    : m_name(name),
      m_maxSize(maxSize)
{
}

Room::Settings::Settings(const Settings& other)
    : m_name(other.m_name),
      m_maxSize(other.m_maxSize),
      m_roomId(other.m_roomId)
{
}

Room::Settings::Settings(Settings&& other)
    : m_name(std::move(other.m_name)),
      m_maxSize(std::move(other.m_maxSize)),
      m_roomId(std::move(other.m_roomId))
{
}

Room::Settings& Room::Settings::operator=(const Settings& other)
{
    if (this != &other)
    {
        m_name = other.m_name;
        m_maxSize = other.m_maxSize;
        m_roomId = other.m_roomId;
    }
    return *this;
}

Room::Settings& Room::Settings::operator=(Settings&& other)
{
    if (this != &other)
    {
        m_name = std::move(other.m_name);
        m_maxSize = std::move(other.m_maxSize);
        m_roomId = std::move(other.m_roomId);
    }
    return *this;
}

Room::Room(const Settings& settings)
    : m_settings(settings)
{
}

Room::Room(Room&& other)
    : m_settings(std::move(other.m_settings))
{
}

Room& Room::operator=(Room&& other)
{
    if (this != &other)
    {
        m_settings = std::move(other.m_settings);
    }
    return *this;
}

} // namespace nexilis
