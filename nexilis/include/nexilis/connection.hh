#ifndef NEXILIS_CONNECTION_HH
#define NEXILIS_CONNECTION_HH

#include <nexilis/protocol.hh>

#include <string>

namespace nexilis
{

// This class acts as an abstraction for different clients.
// Maybe even should be called "Client".

class Connection
{
public:
    /// Constructor.
    Connection(std::string ip_address) noexcept
        : m_ip_address(ip_address)
    {
    }

    /// Move constructor.
    Connection(Connection&& other) noexcept
        : m_ip_address(other.m_ip_address)
    {
    }

    /// Deleted copy constructor.
    Connection(const Connection& other) = delete;

    /// Deleted copy assignment operator.
    Connection& operator=(const Connection other) = delete;

    /// Operator overload for comparison operator.
    bool operator==(const Connection& other) const
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

private:
    std::string m_ip_address;

    unsigned short m_upd_port;
};

} // namespace nexilis

#endif
