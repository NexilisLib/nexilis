#ifndef NEXILIS_CONNECTION_STORAGE_HH
#define NEXILIS_CONNECTION_STORAGE_HH

#include <nexilis/client.hh>

#include <vector>

namespace nexilis
{

class ClientStorage
{
public:
    static void add(Client&& client);

    static bool contains(size_t id);

    static std::vector<Client>& getAllClients();

    static Client* getClientById(size_t id);

    static std::vector<Client*> getClientsByIpAddress(const std::string& ip_address);
private:
    static std::vector<Client> m_clients;

};

} // namespace nexilis

#endif
