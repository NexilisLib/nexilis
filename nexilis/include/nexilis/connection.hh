#ifndef NEXILIS_CONNECTION_HH
#define NEXILIS_CONNECTION_HH

#include <string>

namespace nexilis
{

class Connection
{
public:
    /// Constructor.
    Connection(std::string ip_address) noexcept :
        m_ip_address(ip_address)
    {
    }

    /// Move constructor.
    Connection(Connection&& other) noexcept :
        m_ip_address(other.m_ip_address)
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
    std::string getIPAddress() const { return m_ip_address; }

private:
    std::string m_ip_address;
};

}

#endif
