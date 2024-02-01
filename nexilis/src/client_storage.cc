#include <nexilis/client_storage.hh>

namespace nexilis
{

std::vector<Client> ClientStorage::m_clients = {};

std::vector<Client>& ClientStorage::getAllClients()
{
    return m_clients;
}

void ClientStorage::add(Client&& client)
{
    m_clients.emplace_back(std::move(client));
    Log::info("New client, total amount = ", m_clients.size());
}

bool ClientStorage::contains(size_t id)
{
    return std::find_if(m_clients.begin(), m_clients.end(),
                        [id](const Client& client)
                        {
                            return client.getId() == id;
                        }) != m_clients.end();
}

Client* ClientStorage::getClientById(size_t id)
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

std::vector<Client*> ClientStorage::getClientsByIpAddress(const std::string& ip_address)
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



}
