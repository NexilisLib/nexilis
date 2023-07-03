#ifndef NEXILIS_CONNECTION_HH
#define NEXILIS_CONNECTION_HH

#include <string>

namespace nexilis
{

class Connection
{
public:
    Connection(std::string ip_address, unsigned short port_number) :
        m_ip_address(ip_address), m_port_number(port_number)
    {
    }

    std::string getIPAddress() const { return m_ip_address; }
    unsigned short getPortNumber() const { return m_port_number; }

private:
    std::string m_ip_address;
    unsigned short m_port_number;
};

}

#endif
