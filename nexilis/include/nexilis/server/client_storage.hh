#ifndef NEXILIS_CLIENT_STORAGE_HH
#define NEXILIS_CLIENT_STORAGE_HH

#include <nexilis/server/user.hh>

namespace nexilis::server
{

/// Nexilis Server-side API.
/// Creating static lifetime for the clients in the server context.
class ClientStorage
{
public:
    static void add(User&& client);

    static bool contains(uint64_t id);

    static std::vector<User>& getAllClients();

    static User* getClientById(uint64_t id);

    static std::vector<User*> getClientsByIpAddress(const std::string& ip_address);

private:
    static std::vector<User> m_clients;
};

} // namespace nexilis::server

#endif
