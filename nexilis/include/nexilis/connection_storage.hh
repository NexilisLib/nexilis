#ifndef NEXILIS_CONNECTION_STORAGE_HH
#define NEXILIS_CONNECTION_STORAGE_HH

#include <nexilis/logger/console_handler.hh>
#include <nexilis/logger/logger.hh>
#include <nexilis/connection.hh>
#include <nexilis/log.hh>

#include <vector>
#include <algorithm>

namespace nexilis
{

class ConnectionStorage
{
public:
    static void add(Connection&& connection)
    {
        Log::info("new connection");
        m_connections.emplace_back(std::move(connection));
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
