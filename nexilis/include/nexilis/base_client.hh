#ifndef NEXILIS_BASE_CLIENT_HH
#define NEXILIS_BASE_CLIENT_HH

#include <cstdint>

namespace nexilis
{

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

    /// Get the id of the client.
    uint64_t getId() const
    {
        return m_id;
    }

    void setId(uint64_t id)
    {
        m_id = id;
    }

private:
    uint64_t m_id;
};

}

#endif
