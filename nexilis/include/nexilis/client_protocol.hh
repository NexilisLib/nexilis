#ifndef NEXILIS_CLIENT_PROTOCOL_HH
#define NEXILIS_CLIENT_PROTOCOL_HH

#include <nexilis/log.hh>

namespace nexilis
{

class ClientProtocol
{
public:
    size_t getClientId()
    {
        if (m_client_id_set)
        {
            return m_client_id;
        }
        else
        {
            Log::error("Cannot get unset client ID!");
            return 0;
        }
    }

    void setClientId(size_t clientId)
    {
        m_client_id = clientId;
        m_client_id_set = true;
    }

private:
    size_t m_client_id;
    bool m_client_id_set = false;
};

}

#endif
