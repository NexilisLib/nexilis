#ifndef NEXILIS_SERVER_MANAGER_HH
#define NEXILIS_SERVER_MANAGER_HH

#include <nexilis/authentication.hh>
#include <nexilis/command.hh>

namespace nexilis
{

class ServerManager
{
public:
    void setAuthentication(const Authentication& authentication)
    {
        m_authentication = authentication;
        Command::setAuthentication(m_authentication);
    }

private:
    Authentication m_authentication;
};

}

#endif
