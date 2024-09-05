#ifndef NEXILIS_BASE_CLIENT_HH
#define NEXILIS_BASE_CLIENT_HH

#include <cstdint>
#include <string>
#include <cassert>

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

protected:
    uint64_t getId() const
    {
        return m_id;
    }

    void setId(uint64_t id)
    {
        m_id = id;
    }

    std::string getUsername() const
    {
        return m_username;
    }

    void setUsername(const std::string& username)
    {
        m_username = username;
    }

private:
    uint64_t m_id;
    std::string m_username;
};

}

#endif
