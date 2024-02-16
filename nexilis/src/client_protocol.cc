#include <nexilis/client_protocol.hh>

namespace nexilis
{

ClientProtocol::ClientProtocol(ClientAPI* api) :
    m_api(api)
{
}

ClientProtocol::ClientProtocol(ClientProtocol&& other) :
    m_api(other.m_api)
{
    other.m_api = nullptr;
}

ClientProtocol& ClientProtocol::operator=(ClientProtocol&& other)
{
    if (this != &other)
    {
        m_api = other.m_api;
        other.m_api = nullptr;
    }
    return *this;
}

}