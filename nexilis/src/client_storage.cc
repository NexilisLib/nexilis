#include <nexilis/client_storage.hh>

namespace nexilis
{

std::vector<Client> ClientStorage::m_clients = {};

std::vector<Client>& ClientStorage::getAllClients()
{
    return m_clients;
}

}
