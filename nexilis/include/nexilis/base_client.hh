#ifndef NEXILIS_BASE_CLIENT_HH
#define NEXILIS_BASE_CLIENT_HH

#include <cassert>
#include <cstdint>
#include <string>

#include <nexilis/object/object_2d.hh>
#include <nexilis/object/object_3d.hh>

namespace nexilis
{

/// BaseClient offers common functionality between server- and clientside client objects.
class BaseClient
{
public:
    /// Constructor.
    explicit BaseClient(uint64_t id);

    /// Move constructor.
    BaseClient(BaseClient&& other) noexcept;

    /// Move assignment operator.
    BaseClient& operator=(BaseClient&& other) noexcept;

    /// Copy constructor.
    BaseClient(const BaseClient& other)
        : m_id(other.m_id),
          m_username(other.m_username),
          m_object2D(other.m_object2D),
          m_object3D(other.m_object3D)
    {
    }

    /// Copy assignment operator.
    BaseClient& operator=(const BaseClient& other)
    {
        if (this != &other)
        {
            m_id = other.m_id;
            m_username = other.m_username;
            m_object2D = other.m_object2D;
            m_object3D = other.m_object3D;
        }
        return *this;
    }

    uint64_t getId() const
    {
        return m_id;
    }

    const std::string& getUsername() const
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
