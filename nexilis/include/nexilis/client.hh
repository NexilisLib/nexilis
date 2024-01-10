#ifndef NEXILIS_CLIENT_HH
#define NEXILIS_CLIENT_HH

#include <nexilis/protocol.hh>

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
    Client(Client&& other) noexcept
        : m_ip_address(other.m_ip_address)
    {
    }

    /// Deleted copy constructor.
    Client(const Client& other) = delete;

    /// Deleted copy assignment operator.
    Client& operator=(const Client& other) = delete;

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

    unsigned short getUdpPort()
    {
        return m_upd_port;
    }

    void setAccess(bool hasAccess)
    {
        m_hasAccess = hasAccess;
    }

    bool hasAccess()
    {
        return m_hasAccess;
    }

private:
    std::string m_ip_address;

    unsigned short m_upd_port;

    bool m_hasAccess = false;
};

} // namespace nexilis

#endif
