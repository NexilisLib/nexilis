#ifndef NEXILIS_CLIENT_STORAGE_HH
#define NEXILIS_CLIENT_STORAGE_HH

#include <nexilis/server/user.hh>

namespace nexilis::server
{

/// Static lifetime for the clients in the server context.
class ClientStorage
{
public:
    static void add(std::unique_ptr<User> client);

    static bool contains(uint64_t id);

    static std::vector<std::unique_ptr<User>>& getAllClients();

    static User* getClientById(uint64_t id);

    static std::vector<User*> getClientsByIpAddress(const std::string& ip_address);

    static void clear();

private:
    static std::vector<std::unique_ptr<User>> m_clients;
};

} // namespace nexilis::server

#endif
