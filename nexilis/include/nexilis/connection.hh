#ifndef NEXILIS_CONNECTION_HH
#define NEXILIS_CONNECTION_HH

#include "websocket/websocket_macros.hh"

#include <string>

namespace nexilis
{

class Connection
{
public:
    /// Constructor.
    Connection(wpp_websocket& websocket, wpp_connection& connection, std::string ip_address) noexcept :
        m_websocket(websocket),
        m_connection(connection),
        m_ip_address(ip_address)
    {
    }

    /// Move constructor.
    Connection(Connection&& other) noexcept :
        m_websocket(other.m_websocket),
        m_connection(other.m_connection),
        m_ip_address(other.m_ip_address)
    {
    }

    Connection(const Connection& other) = delete;
    Connection& operator=(const Connection other) = delete;

    wpp_websocket& getWppServer() const { return m_websocket; }
    wpp_connection& getWppConnection() const { return m_connection; }

    std::string getIPAddress() const { return m_ip_address; }

private:
    wpp_websocket& m_websocket;
    wpp_connection& m_connection;

    std::string m_ip_address;
};

}

#endif
