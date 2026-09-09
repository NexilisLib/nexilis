#ifndef NEXILIS_GAME_ITEM_HH
#define NEXILIS_GAME_ITEM_HH

#include <nexilis/nx_data.hh>
#include <nexilis/types/vector3.hh>

#include <memory>
#include <mutex>
#include <string>

namespace nexilis
{

class GameItem
{
public:
    GameItem(uint64_t id, const std::string& type, const Vector3f& position,
             const Vector3f& dimensions, const std::string& status,
             const std::string& filepath = "");

    GameItem(const GameItem& other)
        : m_id(other.m_id),
          m_type(other.m_type),
          m_position(other.m_position),
          m_dimensions(other.m_dimensions),
          m_status(other.m_status),
          m_filepath(other.m_filepath),
          m_mutex(std::make_unique<std::mutex>())
    {
    }

    GameItem& operator=(const GameItem& other)
    {
        if (this != &other)
        {
            m_id = other.m_id;
            m_type = other.m_type;
            m_position = other.m_position;
            m_dimensions = other.m_dimensions;
            m_status = other.m_status;
            m_filepath = other.m_filepath;
            if (!m_mutex)
            {
                m_mutex = std::make_unique<std::mutex>();
            }
        }
        return *this;
    }

    GameItem(GameItem&& other) noexcept;
    GameItem& operator=(GameItem&& other) noexcept;

    uint64_t getId() const;
    std::string getType() const;
    Vector3f getPosition() const;
    Vector3f getDimensions() const;
    std::string getStatus() const;
    std::string getFilepath() const;

    void setPosition(const Vector3f& pos);
    void setDimensions(const Vector3f& dim);
    void setStatus(const std::string& status);

private:
    uint64_t m_id;
    std::string m_type;
    Vector3f m_position;
    Vector3f m_dimensions;
    std::string m_status;
    std::string m_filepath;
    mutable std::unique_ptr<std::mutex> m_mutex;
};

} // namespace nexilis

#endif
