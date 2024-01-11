#ifndef NEXILIS_CONNECTION_STORAGE_HH
#define NEXILIS_CONNECTION_STORAGE_HH

#include <nexilis/client.hh>
#include <nexilis/log.hh>
#include <nexilis/logger/console_handler.hh>
#include <nexilis/logger/logger.hh>

#include <algorithm>
#include <vector>

namespace nexilis
{

class ClientStorage
{
public:
    static void add(Client&& client)
    {
        Log::info("new connection");
        Log::info("Clients amount =", m_clients.size());
        m_clients.emplace_back(std::move(client));
    }

    static bool contains(Client& client)
    {
        return std::find(m_clients.begin(), m_clients.end(), client) != m_clients.end();
    }

    static std::vector<Client>& getAllClients();

private:
    static std::vector<Client> m_clients;
};

} // namespace nexilis

#endif
