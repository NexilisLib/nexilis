#ifndef NEXILIS_CLIENT_HH
#define NEXILIS_CLIENT_HH

#include <cstdint>
#include <nexilis/common/util.hh>
#include <nexilis/nexilis_macros.hh>

#include <string>

namespace nexilis
{

// This class acts as an abstraction for different clients.

class Client
{
public:
    /// Constructor.
    Client(std::string ip_address) noexcept
        : m_ip_address(ip_address)
    {
    }

    /// Move constructor.
    Client(Client&& other)
        : m_ip_address(std::move(other.m_ip_address)),
          m_username(std::move(other.m_username)),
          m_id(std::move(other.m_id)),
          m_roomId(std::move(other.m_roomId)),
          m_upd_port(std::move(other.m_upd_port)),
          m_hasRootAccess(std::move(other.m_hasRootAccess)),
          m_hasCommonAccess(std::move(other.m_hasCommonAccess))
    {
    }

    /// Move assignment operator.
    Client& operator=(Client&& other)
    {
        if (this != &other)
        {
            m_ip_address = std::move(other.m_ip_address);
            m_username = std::move(other.m_username);
            m_id = std::move(other.m_id);
            m_roomId = std::move(other.m_roomId);
            m_upd_port = std::move(other.m_upd_port);
            m_hasRootAccess = std::move(other.m_hasRootAccess);
            m_hasCommonAccess = std::move(other.m_hasCommonAccess);
        }
        return *this;
    }

    /// Deleted copy constructor.
    Client(const Client& other) = delete;

    /// Deleted copy assignment operator.
    Client& operator=(const Client& other) = delete;

    /// Destructor.
    ~Client()
    {
    }

    /// Operator overload for comparison operator.
    bool operator==(const Client& other) const
    {
        return m_ip_address == other.getIPAddress();
    }

    /// Getter for the ip address.
    /// \return The ip address of the connection.
    std::string getIPAddress() const
    {
        return m_ip_address;
    }

    void setUdpPort(unsigned short udpPort)
    {
        m_upd_port = udpPort;
    }

    unsigned short getUdpPort() const
    {
        return m_upd_port;
    }

    void setRootAccess(bool hasAccess)
    {
        m_hasRootAccess = hasAccess;
    }

    bool hasRootAccess() const
    {
        return m_hasRootAccess;
    }

    void setCommonAccess(bool hasAccess)
    {
        m_hasCommonAccess = hasAccess;
    }

    bool hasCommonAccess() const
    {
        return m_hasCommonAccess;
    }

    uint64_t getId() const
    {
        return m_id;
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
        m_username = username;
    }

    std::string getUsername() const
    {
        return m_username;
    }

private:
    std::string m_ip_address;
    std::string m_username;
    uint64_t m_id = Util::getRandomUint64();
    uint64_t m_roomId = 0;

private:
    unsigned short m_upd_port;

    bool m_hasRootAccess = false;
    bool m_hasCommonAccess = false;
};

} // namespace nexilis

#endif
