#ifndef NEXILIS_CONNECTION_HH
#define NEXILIS_CONNECTION_HH

#include "websocket/websocket_macros.hh"

#include <string>

namespace nexilis
{

class Connection
{
public:
    Connection(wpp_websocket& websocket, wpp_connection& connection, std::string ip_address, unsigned short port_number) :
        m_websocket(websocket), m_connection(connection), m_ip_address(ip_address), m_port_number(port_number)
    {
    }

    wpp_websocket& getWppServer() const { return m_websocket; }
    wpp_connection& getWppConnection() const { return m_connection; }
    std::string getIPAddress() const { return m_ip_address; }
    unsigned short getPortNumber() const { return m_port_number; }

private:
    wpp_websocket& m_websocket;
    wpp_connection& m_connection;

    std::string m_ip_address;
    unsigned short m_port_number;
};

}

#endif
