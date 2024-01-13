#ifndef NEXILIS_CLIENT_API_HH
#define NEXILIS_CLIENT_API_HH

#include <cstddef>

namespace nexilis
{

class ClientAPI
{
public:
    ClientAPI(size_t client_id) : m_client_id(client_id)
    {
    }

protected:
    size_t getClientId()
    {
        return m_client_id;
    }

private:
    size_t m_client_id;
};

}

#endif
