#ifndef NEXILIS_USER_HH
#define NEXILIS_USER_HH

#include <nexilis/base_client.hh>
#include <nexilis/common/util.hh>
#include <nexilis/nexilis_macros.hh>
#include <nexilis/object2d.hh>

namespace nexilis
{

/// Internal client abstraction for server.
class User : public BaseClient
{
public:
    /// Constructor.
    User(uint64_t id, std::string ip_address) noexcept;

    /// Move constructor.
    User(User&& other);

    /// Move assignment operator.
    User& operator=(User&& other);

    /// Deleted copy constructor.
    User(const User& other) = delete;

    /// Deleted copy assignment operator.
    User& operator=(const User& other) = delete;

    /// Operator overload for comparison operator.
    bool operator==(const User& other) const
    {
        return m_ip_address == other.getIPAddress();
    }

    /// Getter for the ip address.
    /// \return The ip address of the connection.
    std::string getIPAddress() const
    {
        return m_ip_address;
    }

    void setRootAccess(bool hasAccess)
    {
        m_hasRootAccess = hasAccess;
    }

    bool hasRootAccess() const
    {
        return m_hasRootAccess;
    }

    /// Access redeemed by the passphrase.
    void setCommonAccess(bool hasAccess)
    {
        m_hasCommonAccess = hasAccess;
    }

    /// Passphrase has been initialized correctly for server.
    bool hasCommonAccess() const
    {
        return m_hasCommonAccess;
    }

    void setId(uint64_t id)
    {
        assert(hasRootAccess());
        BaseClient::setId(id);
    }

    uint64_t getId() const
    {
        return BaseClient::getId();
    }

    void setRoomId(uint64_t roomId)
    {
        m_roomId = roomId;
    }

    uint64_t getRoomId() const
    {
        return m_roomId;
    }

    void setUsername(const std::string& username)
    {
        BaseClient::setUsername(username);
    }

    std::string getUsername() const
    {
        return BaseClient::getUsername();
    }

    Object2D& getObject2D()
    {
        return m_object2D;
    }

    /// Protocol specific stuff.

    // Boost TCP

    /// Is boost TCP send function set?
    bool isBoostTCPSet()
    {
        return m_boostTCPSendToClient != nullptr;
    }

    /// Set the boost TCP send function.
    void setBoostTCPSend(const std::function<void(const std::vector<uint8_t>&)>& sendFunction)
    {
        m_boostTCPSendToClient = sendFunction;
    }

    /// Send data using boost TCP.
    bool boostTCPSend(std::vector<uint8_t> data);

    // Boost UDP

    /// Is boost UDP send function set?
    bool isBoostUDPSet()
    {
        return m_boostUDPSendToClient != nullptr;
    }

    /// Set the boost UDP send function.
    void setBoostUDPSend(const std::function<void(const std::vector<uint8_t>&)>& sendFunction)
    {
        m_boostUDPSendToClient = sendFunction;
    }

    /// Send data using boost UDP.
    bool boostUDPSend(std::vector<uint8_t> data);

private:
    // General
    std::string m_ip_address;
    std::string m_username;
    uint64_t m_roomId = 0;

    /// The 2D properties of the client.
    Object2D m_object2D;
private:
    std::function<void(std::vector<uint8_t>)> m_boostTCPSendToClient = nullptr;
    std::function<void(std::vector<uint8_t>)> m_boostUDPSendToClient = nullptr;

private:
    /// Access area.
    bool m_hasRootAccess = false;
    bool m_hasCommonAccess = false;
};

} // namespace nexilis

#endif
