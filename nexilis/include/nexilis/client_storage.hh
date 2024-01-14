#ifndef NEXILIS_CONNECTION_STORAGE_HH
#define NEXILIS_CONNECTION_STORAGE_HH

#include <nexilis/client.hh>
#include <nexilis/log.hh>

#include <algorithm>
#include <vector>

namespace nexilis
{

class ClientStorage
{
public:
    static void add(Client&& client)
    {
        m_clients.emplace_back(std::move(client));
        Log::info("New client, total amount = ", m_clients.size());
    }

    static bool contains(size_t id)
    {
        return std::find_if(m_clients.begin(), m_clients.end(),
                            [id](const Client& client)
                            {
                                return client.getId() == id;
                            }) != m_clients.end();
    }

    static std::vector<Client>& getAllClients();

    static Client* getClientById(size_t id)
    {
        auto it = std::find_if(m_clients.begin(), m_clients.end(),
                               [id](const Client& client) {
                                   return client.getId() == id;
                               });

        if (it != m_clients.end())
        {
            return &(*it);
        }
        else
        {
            return nullptr;
        }
    }

    static std::vector<Client*> getClientsByIpAddress(const std::string& ip_address)
    {
        std::vector<Client*> result;

        for (auto& client : m_clients)
        {
            if (client.getIPAddress() == ip_address) 
            {
                result.push_back(&client);
            }
        }

        return result;
    }

private:
    static std::vector<Client> m_clients;

};

} // namespace nexilis

#endif
