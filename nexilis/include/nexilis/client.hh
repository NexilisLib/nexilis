#ifndef NEXILIS_CLIENT_HH
#define NEXILIS_CLIENT_HH

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
    Client(std::string ip_address) noexcept : 
        m_ip_address(ip_address),
        m_id(Util::getRandomSizeT(0, NEXILIS_MAX))
    {
    }

    /// Move constructor.
    Client(Client&& other) :
        m_ip_address(other.m_ip_address),
        m_username(other.m_username),
        m_id(other.m_id),
        m_upd_port(other.m_upd_port),
        m_hasRootAccess(other.m_hasRootAccess),
        m_hasCommonAccess(other.m_hasCommonAccess)
    {
    }

    /// Move assignment operator.
    Client& operator=(Client&& other)
    {
        if (this != &other)
        {
            m_ip_address = other.m_ip_address;
            m_username = other.m_username;
            m_id = other.m_id;
            m_upd_port = other.m_upd_port;
            m_hasRootAccess = other.m_hasRootAccess;
            m_hasCommonAccess = other.m_hasCommonAccess;
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

    size_t getId() const
    {
        return m_id;
    }

    void setUsername(const std::string& username)
    {
        m_username = username;
    }

    std::string getUsername() const
    {
        return m_username;
    }

    // TODO implment validation for different protocols.

private:
    std::string m_ip_address;
    std::string m_username;
    size_t m_id;

private:
    unsigned short m_upd_port;

    bool m_hasRootAccess = false;
    bool m_hasCommonAccess = false;
};

} // namespace nexilis

#endif
