#ifndef NEXILIS_CONNECTION_STORAGE_HH
#define NEXILIS_CONNECTION_STORAGE_HH

#include <nexilis/connection.hh>

#include <vector>
#include <algorithm>
#include <iostream>

namespace nexilis
{

class ConnectionStorage
{
public:
    static void add(Connection&& connection)
    {
        m_connections.emplace_back(std::move(connection));
        std::cout << "Added new connection: " << connection.getIPAddress() << std::endl;
    }

    static bool contains(Connection& connection)
    {
        return std::find(m_connections.begin(), m_connections.end(), connection) != m_connections.end();
    }

private:
    static std::vector<Connection> m_connections;
};

}

#endif
