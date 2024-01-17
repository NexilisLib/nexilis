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
};

}

#endif
