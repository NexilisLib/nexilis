#ifndef NEXILIS_SERVER_MANAGER_HH
#define NEXILIS_SERVER_MANAGER_HH

#include <nexilis/authentication.hh>
#include <nexilis/command.hh>

namespace nexilis
{

class ServerManager
{
public:
    void setAuthentication(Authentication& authentication)
    {
        Command::setAuthentication(authentication);
    }

    void setMaxAmountOfClients(size_t amount)
    {
        m_maxClients = amount;
    }

    static size_t getMaxAmountOfClients()
    {
        return m_maxClients;
    }

private:
    /// Max amount of clients in the server, default 1000.
    static size_t m_maxClients;
};

}

#endif
