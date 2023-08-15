#ifndef NEXILIS_CORE_HH
#define NEXILIS_CORE_HH

#include <string>

namespace nexilis
{

class Core
{
public:
    Core(const std::string& serverName) :
        m_serverName(serverName)
    {
    }

    std::string getServerName() { return m_serverName; }

private:
    std::string m_serverName;

};

}

#endif
