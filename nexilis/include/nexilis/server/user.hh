#ifndef NEXILIS_USER_HH
#define NEXILIS_USER_HH

#include <cstdint>
#include <nexilis/base_client.hh>
#include <nexilis/nexilis_constants.hh>
#include <nexilis/object/object2d.hh>
#include <nexilis/util.hh>

namespace nexilis::server
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

    /// Protocol specific stuff.

    /// \defgroup UserBoostTCP Sending messages via Boost TCP.

    /// Is boost TCP send function set?
    /// \ingroup UserBoostTCP
    bool isBoostTCPSet()
    {
        return m_boostTCPSendToClient != nullptr;
    }

    /// Set the boost TCP send function.
    /// \ingroup UserBoostTCP
    void setBoostTCPSend(const std::function<void(const nx_data&)>& sendFunction)
    {
        m_boostTCPSendToClient = sendFunction;
    }

    /// Send data using boost TCP.
    /// \ingroup UserBoostTCP
    bool boostTCPSend(const nx_data& data);

    ///\defgroup UserBoostUDP Sending messages via Boost UDP

    /// Is boost UDP send function set?
    /// \ingroup UserBoostUDP
    bool isBoostUDPSet()
    {
        return m_boostUDPSendToClient != nullptr;
    }

    /// Set the boost UDP send function.
    /// \ingroup UserBoostUDP
    void setBoostUDPSend(const std::function<void(const nx_data&)>& sendFunction)
    {
        m_boostUDPSendToClient = sendFunction;
    }

    /// Send data using boost UDP.
    /// \ingroup UserBoostUDP
    bool boostUDPSend(const nx_data& data);

    /// \defgroup UserUnixStream Send data using unix stream soccets.

    /// Is User unixstream send function set?
    /// \ingroup UserUnixStream
    bool isUnixStreamSet()
    {
        return m_unixStreamSendToClient != nullptr;
    }

    /// \ingroup UserUnixStream
    void setUnixStreamSend(const std::function<void(const nx_data&)>& sendFunction)
    {
        m_unixStreamSendToClient = sendFunction;
    }

    /// \ingroup UserUnixStream
    bool unixStreamSend(const nx_data& data);

private:
    // General
    std::string m_ip_address;
    std::string m_username;
    uint64_t m_roomId = 0;

private:
    std::function<void(nx_data)> m_boostTCPSendToClient = nullptr;
    std::function<void(nx_data)> m_boostUDPSendToClient = nullptr;
    std::function<void(nx_data)> m_unixStreamSendToClient = nullptr;

private:
    /// Access area.
    bool m_hasRootAccess = false;
    bool m_hasCommonAccess = false;
};

} // namespace nexilis::server

#endif
