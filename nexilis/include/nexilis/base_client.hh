#ifndef NEXILIS_BASE_CLIENT_HH
#define NEXILIS_BASE_CLIENT_HH

#include <cassert>
#include <cstdint>
#include <string>

#include <nexilis/object/object2d.hh>
#include <nexilis/object/object3d.hh>

namespace nexilis
{

/// BaseClient offers common functionality between server- and clientside client objects.
class BaseClient
{
public:
    /// Constructor.
    explicit BaseClient(uint64_t id);

    /// Move constructor.
    BaseClient(BaseClient&& other);

    /// Move assignment operator.
    BaseClient& operator=(BaseClient&& other);

    /// Deleted copy constructor.
    BaseClient(const BaseClient& other) = delete;

    /// Deleted copy assignment operator.
    BaseClient& operator=(const BaseClient& other) = delete;

    uint64_t getId() const
    {
        return m_id;
    }

    std::string getUsername() const
    {
        return m_username;
    }

    Object2D& getObject2D()
    {
        return m_object2D;
    }

    Object3D& getObject3D()
    {
        return m_object3D;
    }

protected:
    void setId(uint64_t id)
    {
        m_id = id;
    }

    void setBaseUsername(const std::string& username)
    {
        m_username = username;
    }

private:
    uint64_t m_id;
    std::string m_username;
    Object2D m_object2D;
    Object3D m_object3D;
};

} // namespace nexilis

#endif
