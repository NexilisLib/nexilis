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

    GameItem(const GameItem&) = delete;
    GameItem& operator=(const GameItem&) = delete;

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
