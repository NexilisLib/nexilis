#ifndef NEXILIS_BASE_CLIENT_HH
#define NEXILIS_BASE_CLIENT_HH

#include <cstdint>
#include <string>
#include <cassert>

#include <nexilis/object2d.hh>
#include <nexilis/object3d.hh>

namespace nexilis
{

/// BaseClient offers common functionality between server- and clientside client objects.
class BaseClient
{
public:
    /// Constructor.
    BaseClient(uint64_t id);

    /// Move constructor.
    BaseClient(BaseClient&& other);

    /// Move assignment operator.
    BaseClient& operator=(BaseClient&& other);

    /// Deleted copy constructor.
    BaseClient(const BaseClient& other) = delete;

    /// Deleted copy assignment operator.
    BaseClient& operator=(const BaseClient& other) = delete;

    virtual uint64_t getId() const
    {
        return m_id;
    }

    virtual std::string getUsername() const
    {
        return m_username;
    }

    virtual Object2D& getObject2D()
    {
        return m_object2D;
    }

protected:
    void setId(uint64_t id)
    {
        m_id = id;
    }

       void setUsername(const std::string& username)
    {
        m_username = username;
    }

private:
    Object2D m_object2D;
    uint64_t m_id;
    std::string m_username;
};

}

#endif
