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
        id_counter += 1;
        m_id = id_counter;
    }

    /// Move constructor.
    Client(Client&& other) noexcept :
        m_ip_address(other.m_ip_address),
        m_upd_port(other.m_upd_port),
        m_hasRootAccess(other.m_hasRootAccess),
        m_hasCommonAccess(other.m_hasCommonAccess),
        m_id(other.m_id)
    {
    }

    /// Deleted copy constructor.
    Client(const Client& other) = delete;

    /// Deleted copy assignment operator.
    Client& operator=(const Client& other) = delete;

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

    unsigned short getUdpPort()
    {
        return m_upd_port;
    }

    void setRootAccess(bool hasAccess)
    {
        m_hasRootAccess = hasAccess;
    }

    bool hasRootAccess()
    {
        return m_hasRootAccess;
    }

    void setCommonAccess(bool hasAccess)
    {
        m_hasCommonAccess = hasAccess;
    }

    bool hasCommonAccess()
    {
        return m_hasCommonAccess;
    }

private:
    std::string m_ip_address;
    unsigned short m_upd_port;

    bool m_hasRootAccess = false;
    bool m_hasCommonAccess = false;

    size_t m_id;

private:
    static size_t id_counter;
};

} // namespace nexilis

#endif
