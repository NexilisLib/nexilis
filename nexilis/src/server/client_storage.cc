#include <nexilis/server/client_storage.hh>

#include <nexilis/logger/log.hh>

namespace nexilis
{

std::vector<User> ClientStorage::m_clients = {};

std::vector<User>& ClientStorage::getAllClients()
{
    return m_clients;
}

void ClientStorage::add(User&& client)
{
    m_clients.emplace_back(std::move(client));
    Log::info("New client, total amount = ", m_clients.size());
}

bool ClientStorage::contains(size_t id)
{
    return std::find_if(m_clients.begin(), m_clients.end(),
                        [id](const User& client)
                        {
                            return client.getId() == id;
                        }) != m_clients.end();
}

User* ClientStorage::getClientById(uint64_t id)
{
    auto it = std::find_if(m_clients.begin(), m_clients.end(),
                           [id](const User& client)
                           {
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

std::vector<User*> ClientStorage::getClientsByIpAddress(const std::string& ip_address)
{
    std::vector<User*> result;

    for (auto& client : m_clients)
    {
        if (client.getIPAddress() == ip_address)
        {
            result.push_back(&client);
        }
    }

    return result;
}

} // namespace nexilis
